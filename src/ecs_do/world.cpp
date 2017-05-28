#include "world.hpp"

#include "systems.hpp"
#include "entity.hpp"

world::world()
	: m_next_index(0)
	, m_count(0)
{  }

uint64_t world::create()
{
	++m_count;
	if(m_freelist.empty())
		return (m_next_index++ << VERSION_BITS);

	uint64_t id = m_freelist.front();
	m_freelist.pop_front();
	return id;
}

void world::kill(uint64_t e)
{
	m_dead.push_back(e);
}

void world::update(double dt)
{
	update_health(this, dt);
	update_reproduction(this, dt);
	update_movement(this, dt);

	for(uint64_t e : m_dead)
	{
		pos.kill(e);
		hea.kill(e);
		rep.kill(e);
		mov.kill(e);
		spe.kill(e);

		--m_count;
		if(version(e) < ((1<<VERSION_BITS) - 1))
			m_freelist.push_back(e+1);
	}
	m_dead.clear();
}
