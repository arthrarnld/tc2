#include "ood/world.hpp"

#include "common/log.hpp"

entity & world::create(int count)
{
	entity::id id;
	if(!m_freelist.empty())
	{
		id = m_freelist.front();
		m_freelist.pop_front();
	}
	else
	{
		if(m_counter > ((entity::id)1 << INDEX_BITS) - 1)
			fatal("ran out of indices for entities");
		id = m_counter++ << VERSION_BITS;
	}
	m_entities[id] = entity(id);
}

void world::destroy(entity & e)
{
	if(e.get_version() < (1 << VERSION_BITS) - 1)
		m_freelist.push_back((e.get_index() << VERSION_BITS) | (e.get_version() + 1));
	m_entities.erase(e.get_id());
}