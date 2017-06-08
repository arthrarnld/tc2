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

void world::update(double dt)
{
	update_health(this, dt);
	update_reproduction(this, dt);
	update_movement(this, dt);
}

// void world::print()
// {
// 	pos.print();
// 	hea.print();
// 	rep.print();
// 	spe.print();
// 	mov.print();
// }
