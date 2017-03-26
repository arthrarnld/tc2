#include "ood/entity.hpp"

#include "common/log.hpp"

static constexpr uint64_t VERSION_BITS = 8;
static constexpr uint64_t INDEX_BITS = 64 - VERSION_BITS;
std::list<uint64_t> entity::freelist;

entity::entity()
{
	static uint64_t counter = 0;

	if(!freelist.empty())
	{
		m_id = freelist.front();
		freelist.pop_front();
	}
	else
	{
		if(counter > (uint64_t(1) << INDEX_BITS) - 1)
			fatal("entity index space full");
		m_id = counter++ << VERSION_BITS;
	}
}

entity::~entity()
{
	if(get_version() < (1 << VERSION_BITS) - 1)
		freelist.push_back((get_index() << VERSION_BITS) | (get_version() + 1));
}

uint64_t entity::get_id()
{
	return m_id;
}

uint64_t entity::get_index()
{
	return m_id >> VERSION_BITS;
}

uint64_t entity::get_version()
{
	return m_id & ((1 << VERSION_BITS) - 1);
}

bool entity::is_enabled()
{
	return m_enabled;
}

void entity::disable()
{
	m_enabled = false;
}

void entity::enable()
{
	m_enabled = true;
}

void entity::handle_message(message * msg)
{
	for(auto & c : m_components)
		if(c->handle_message(msg))
			break;
}
