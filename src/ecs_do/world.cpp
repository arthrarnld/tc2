#include "world.hpp"

static const uint64_t VERSION_BITS = 8;
static const uint64_t INDEX_BITS = 64 - VERSION_BITS;

uint64_t world::create()
{
	if(m_freelist.empty())
		return (m_next_index++ << VERSION_BITS);

	uint64_t id = m_freelist.front();
	m_freelist.pop_front();
	return id;
}

void world::destroy(uint64_t eid)
{
	uint64_t version = eid & ((1<<VERSION_BITS) - 1);
	if(version < ((1<<VERSION_BITS) - 1))
		m_freelist.push_back(eid+1);
}
