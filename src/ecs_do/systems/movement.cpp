#include "systems/movement.hpp"
#include "common/log.hpp"
#include "common/constants.hpp"
#include "world.hpp"

#include "components/species.hpp"
#include "glm/gtx/rotate_vector.hpp"

static const float inf = std::numeric_limits<float>::infinity();

inline float sqdist(const glm::vec2 & u, const glm::vec2 & v)
{
	return (u.x-v.x)*(u.x-v.x) + (u.y-v.y)*(u.y-v.y);
}

size_t find_closest_of(world * w, uint64_t self, const glm::vec2 & my_pos, int target_species)
{
	position & p = w->pos;
	species & s = w->spe;
	float dist = inf;
	float d;
	size_t curr = nil;
	for(size_t i = 0; i < s.size(); ++i)
	{
		if(s.id[i] == target_species)
		{
			uint64_t current = s.owner[i];
			if(current != self)
			{
				d = sqdist(p.pos[p.lookup(current)], my_pos);
				if(d < dist)
				{
					dist = d;
					curr = current;
				}
			}
		}
	}
	return curr;
}

bool update_movement(world * w, double dt)
{
	movement & m = w->mov;
	position & p = w->pos;
	species & s = w->spe;
	reproduction & r = w->rep;
	health & h = w->hea;

	#ifdef DO_PARTITION_ARRAYS
		// Idle
		for(size_t i = 0; i < m.partitions[0]; )
		{
			uint64_t self = m.owner[i];
			glm::vec2 & my_pos = p.pos[p.lookup(self)];
			int my_species = s.id[s.lookup(self)];
			uint64_t & my_target = m.target[i];

			size_t ridx = r.lookup(self);
			size_t hidx = h.lookup(self);

			if(r.desire[ridx] > 50.0f && r.desire[ridx] > h.hunger[hidx]) {
				my_target = find_closest_of(w, self, my_pos, my_species);
				if(my_target != nil) {
					debug("%llu seeking to mate with %llu", self, my_target);
					// Since seek_mate() may modify the arrays,
					// we need to update i accordingly.
					i = m.seek_mate(i);
					continue;
				}
			}
			if(h.hunger[hidx] > 50.0f) {
				my_target = find_closest_of(w, self, my_pos, h.prey[h.lookup(self)]);
				if(my_target != nil) {
					debug("%llu seeking to eat %llu", self, my_target);
					// Same here
					i = m.seek_food(i);
					continue;
				}
			}
			// No target was found. Wander about randomly.
			glm::vec2 delta = m.direction[i];
			float angle = drand(to_rad(-30), to_rad(30));
			delta = glm::rotate(delta, angle);
			m.direction[i] = glm::normalize(m.direction[i] + delta);
			my_pos += m.direction[i] * m.speed[i] * (float)dt;
			debug("%llu at (%.2f, %.2f)", self, my_pos.x, my_pos.y);
			++i;
		}

		// Seeking food
		for(size_t i = m.partitions[0]; i < m.partitions[1]; )
		{
			uint64_t self = m.owner[i];
			glm::vec2 & my_pos = p.pos[p.lookup(self)];
			uint64_t & my_target = m.target[i];

			glm::vec2 & tgt_pos = p.pos[p.lookup(my_target)];
			if(tgt_pos != my_pos)
			{
				my_pos = glm::length(tgt_pos - my_pos) <= m.speed[i]*(float)dt
					? tgt_pos
					: my_pos + glm::normalize(tgt_pos - my_pos) * m.speed[i] * (float)dt;
				if(tgt_pos == my_pos)
				{
					debug("%llu starting to eat %llu", self, my_target);
					i = h.start_eating(h.lookup(self));
					continue;
				}
			}
			++i;
		}

		// Seeking mate
		for(size_t i = m.partitions[1]; i < m.size(); ++i)
		{
			uint64_t self = m.owner[i];
			glm::vec2 & my_pos = p.pos[p.lookup(self)];
			uint64_t & my_target = m.target[i];

			glm::vec2 & tgt_pos = p.pos[p.lookup(my_target)];
			if(tgt_pos != my_pos)
			{
				my_pos = glm::length(tgt_pos - my_pos) <= m.speed[i]*(float)dt
					? tgt_pos
					: my_pos + glm::normalize(tgt_pos - my_pos) * m.speed[i] * (float)dt;
				if(tgt_pos == my_pos)
				{
					debug("%llu starting to mate with %llu", self, my_target);
					i = r.start_mating(r.lookup(self));
					continue;
				}
			}
			++i;
		}

	#else

		for(size_t i = 0; i < m.size(); ++i)
		{
			uint64_t self = m.owner[i];
			glm::vec2 & my_pos = p.pos[p.lookup(self)];
			int my_species = s.id[s.lookup(self)];
			movement::state_type & my_state = m.state[i];
			uint64_t & my_target = m.target[i];

			if(my_state == movement::IDLE)
			{
				size_t ridx = r.lookup(self);
				size_t hidx = h.lookup(self);

				if(r.desire[ridx] > 50.0f && r.desire[ridx] > h.hunger[hidx]) {
					debug("%llu must mate", self);
					my_target = find_closest_of(w, self, my_pos, my_species);
					if(my_target != nil) {
						m.seek_mate(i);
						debug("%llu seeking to mate with %llu [%d]", self, my_target, my_species);
					}
				}
				if(my_target == nil && h.hunger[hidx] > 50.0f) {
					debug("%llu must eat", self);
					my_target = find_closest_of(w, self, my_pos, h.prey[h.lookup(self)]);
					if(my_target != nil) {
						m.seek_food(i);
						debug("%llu seeking to eat %llu", self, my_target);
					}
				}
				if(my_target == nil) {
					// No target was found. Wander about randomly.
					glm::vec2 delta = m.direction[i];
					float angle = drand(to_rad(-30), to_rad(30));
					delta = glm::rotate(delta, angle);
					m.direction[i] = glm::normalize(m.direction[i] + delta);
					my_pos += m.direction[i] * m.speed[i] * (float)dt;
					debug("%llu at (%.2f, %.2f)", self, my_pos.x, my_pos.y);
					continue;
				}
			}

			glm::vec2 & tgt_pos = p.pos[p.lookup(my_target)];
			if(tgt_pos != my_pos)
			{
				my_pos = glm::length(tgt_pos - my_pos) <= m.speed[i]*(float)dt
					? tgt_pos
					: my_pos + glm::normalize(tgt_pos - my_pos) * m.speed[i] * (float)dt;
				if(tgt_pos == my_pos)
				{
					if(my_state == movement::SEEK_MATE) {
						r.start_mating(r.lookup(self));
						debug("%llu starting to mate with %llu", self, my_target);
					} else if(my_state == movement::SEEK_FOOD) {
						h.start_eating(h.lookup(self));
						debug("%llu starting to eat %llu", self, my_target);
					}
				}
			}
		}
	#endif // DO_PARTITION_ARRAYS

	return true;
}
