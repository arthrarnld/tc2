#include "systems/movement.hpp"
#include "common/log.hpp"
#include "constants.hpp"
#include "world.hpp"

bool update_movement(world * w, double dt)
{
	movement & m = w->mov;
	position & p = w->pos;
	for(size_t i = 0; i < m.size(); ++i)
	{
		uint64_t self = m.owner[i];

		uint64_t target = m.target[i];
		if(target == nil)
			continue;

		size_t pos_idx = p.lookup(self);
		if(pos_idx == nil)
			continue;
		glm::vec2 & my_pos = p.pos[pos_idx];

		pos_idx = p.lookup(target);
		if(pos_idx == nil)
			continue;
		glm::vec2 & their_pos = p.pos[pos_idx];

		my_pos += glm::normalize(their_pos - my_pos) * m.speed[i] * (float)dt;
		debug("[%llu] at (%.2f,%.2f)", self, my_pos.x, my_pos.y);
	}
	return true;
}
