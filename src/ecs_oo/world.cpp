#include "world.hpp"

#include <algorithm>

#include "common/log.hpp"
#include "systems/base_system.hpp"

bool world::update(double dt)
{
	flush_messages();
	for(auto & s : m_systems)
		if(!s->update(*this, dt))
			return false;
	return true;
}

entity & world::create()
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
	return *e;
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

unsigned world::get_entity_count()
{
	return m_entities.size();
}

void world::push_system(base_system *s)
{
	m_systems.emplace_back(s);
}

void world::push_message(base_message * m)
{
	m_messages.emplace_back(m);
}

world::container_type::iterator world::begin()
{
	return m_entities.begin();
}

world::container_type::iterator world::end()
{
	return m_entities.end();
}

void world::for_each(std::function<void(entity&)> && fn)
{
	for(auto & pair : m_entities)
		fn(*(pair.second.get()));
}

void world::flush_messages()
{
	for(auto & m : m_messages)
	{
		for(auto & s : m_systems)
			if(s->handle_message(m.get(), *this))
				break;
	}
	m_messages.clear();

}
