#include "world.hpp"

#include <algorithm>

#include "common/log.hpp"

void world::update(double dt)
{
	health.update(*this, dt);
	reproduction.update(*this, dt);
	movement.update(*this, dt);
}

entity & world::create()
{
	m_entities.emplace_back(new entity());
	return *m_entities.back();
}

void world::create(int count)
{
	for(int i = 0; i < count; ++i)
		create();
}

unsigned world::get_entity_count()
{
	return m_entities.size();
}

world::iterator world::begin()
{
	return m_entities.begin();
}

world::iterator world::end()
{
	return m_entities.end();
}
