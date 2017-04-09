#include "world.hpp"

#include "common/log.hpp"
#include "base_system.hpp"

bool world::update(double dt)
{
	auto begin	= m_entities.begin();
	auto end	= m_entities.end();
	for(auto & s : m_systems)
		if(!s->update(begin, end, dt))
			return false;
}

world::entity_ptr world::create()
{
	entity::id id;
	if(!m_freelist.empty())
	{
		id = m_freelist.front();
		m_freelist.pop_front();
	}
	else
	{
		if(m_counter > ((entity::id)1 << entity::INDEX_BITS) - 1)
			fatal("ran out of indices for entities");
		id = m_counter++ << entity::VERSION_BITS;
	}
	auto e = entity_ptr(new entity(id));
	m_entities[id] = e;
	return e;
}

void world::create(int count)
{
	for(int i = 0; i < count; ++i)
		create();
}

world::entity_ptr world::get(entity::id id)
{
	auto it = m_entities.find(id);
	if(it == m_entities.end())
		return entity_ptr();
	return (*it).second;
}

void world::destroy(entity & e)
{
	if(e.get_version() < (1 << entity::VERSION_BITS) - 1)
		m_freelist.push_back((e.get_index() << entity::VERSION_BITS) | (e.get_version() + 1));
	m_entities.erase(e.get_id());
}
