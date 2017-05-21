#include "systems/attack.hpp"
#include "common/log.hpp"
#include "constants.hpp"
#include "world.hpp"

// uint64_t find_target(size_t self, position & p, team & t)
// {
// 	for(size_t i = 0; i < p.size(); ++i)
// 	{
// 		if(i == self)
// 			continue;
// 		if(!t.contains(p.owner[i]))
// 			continue;
// 		int their_team = t.team_id[i];
// 		if()
// 	}
// }

static const float inf = std::numeric_limits<float>::infinity();

bool update_attack(world * w, double dt)
{
	attack & a = w->att;
	position & p = w->pos;
	team & t = w->tea;
	movement & m = w->mov;
	health & h = w->hea;
	size_t lookup_index;
	for(size_t i = 0; i < a.size(); ++i)
	{
		uint64_t self = a.owner[i];
		float & reload_rem = a.reload_rem[i];

		if(reload_rem > 0)
			reload_rem -= dt;

		if(reload_rem <= 0)
			reload_rem = 0;
		else
			continue;

		lookup_index = p.lookup(self);
		if(lookup_index == nil)
		{
			error("Entity with attack component missing position component");
			continue;
		}
		glm::vec2 & my_pos = p.pos[lookup_index];

		int my_team = t.team_id[t.lookup(self)];

		float dist = inf;
		uint64_t target;
		bool found = false;
		for(size_t ti = 0; ti < p.size(); ++ti)
		{
			uint64_t tgt = p.owner[ti];
			if(self == tgt)
				continue;
			lookup_index = t.lookup(tgt);
			if(lookup_index != nil)
			{
				int their_team = t.team_id[lookup_index];
				if(my_team == their_team)
					continue;
			}
			glm::vec2 & their_pos = p.pos[ti];
			float d = glm::length(their_pos - my_pos);
			if(d < dist) {
				dist = d;
				target = tgt;
				found = true;
			}
		}

		if(found) {
			if(dist <= a.range[i]) {
				debug("%llu attacking %llu", self, target);
				a.state[i] = attack::ENGAGED;
				reload_rem = a.reload[i];

				lookup_index = h.lookup(target);
				if(lookup_index == nil) {
					error("target missing health component");
					continue;
				}
				h.health_points[lookup_index] -= a.damage[i];
				debug("%llu takes %.2f damage [HP = %.2f]", target, a.damage[i], h.health_points[lookup_index]);

				lookup_index = m.lookup(self);
				if(lookup_index != nil)
					m.target[lookup_index] = nil;
			} else if((lookup_index = m.lookup(self)) != nil) {
				if(m.target[lookup_index] != target)
					debug("%llu pursuing %llu", self, target);
				m.target[lookup_index] = target;
			}
		} else {
			a.state[i] = attack::IDLE;
		}
	}
	return true;
}
