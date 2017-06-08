#include "systems/movement_system.hpp"

#include "common/log.hpp"
#include "common/util.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "components/movement_component.hpp"
#include "components/position_component.hpp"
#include "components/species_component.hpp"
#include "components/reproduction_component.hpp"
#include "components/health_component.hpp"

static const float inf = std::numeric_limits<float>::infinity();

entity::id find_closest_of(world & w, int my_id, const glm::vec2 & my_pos, int target_species)
{
	float dist = inf;
	float d;
	entity::id curr = entity::NIL;
	for(auto & p : w)
	{
		auto & e = p.second;
		auto sc = e->get_component<species_component>();
		if(e->get_id() != my_id && sc->id == target_species)
		{
			d = glm::length(e->get_component<position_component>()->position - my_pos);
			if(d < dist)
			{
				dist = d;
				curr = e->get_id();
			}
		}
	}
	return curr;
}

bool movement_system::update(world & w, double dt)
{
	for(auto & p : w)
	{
		auto & e = p.second;
		entity::id my_id = e->get_id();

		if(!e->has_component<movement_component>())
			continue;

		// Movement
		auto mc = e->get_component<movement_component>();
		uint64_t & my_target = mc->target;
		movement_component::state_type & my_state = mc->state;
		// Position
		auto pc = e->get_component<position_component>();
		glm::vec2 & my_pos = pc->position;
		// Reproduction
		auto rc = e->get_component<reproduction_component>();
		// Health
		auto hc = e->get_component<health_component>();
		// Species
		auto sc = e->get_component<species_component>();
		int my_species = sc->id;


		if(my_state == movement_component::IDLE)
		{
			if(rc->desire > 50.0f && rc->desire > hc->hunger)
			{
				debug("%llu [%.2f, %.2f] must mate", my_id, rc->desire, hc->hunger);
				my_target = find_closest_of(w, my_id, my_pos, my_species);
				if(my_target != nil) {
					my_state = movement_component::SEEK_MATE;
				}
				debug("%llu seeking to mate with %llu", my_id, rc->desire, hc->hunger, my_target);
			}
			if(my_target == nil && hc->hunger > 50.0f)
			{
				debug("%llu must eat", my_id, rc->desire, hc->hunger);
				my_target = find_closest_of(w, my_id, my_pos, hc->prey);
				if(my_target != nil) {
					my_state = movement_component::SEEK_FOOD;
					debug("%llu seeking to eat %llu", my_id, rc->desire, hc->hunger, my_target);
				}
			}
			if(my_target == nil) {
				// No target was found. Wander about randomly.
				glm::vec2 delta = mc->direction;
				float angle = drand(to_rad(-30), to_rad(30));
				delta = glm::rotate(delta, angle);
				mc->direction = glm::normalize(mc->direction + delta);
				my_pos += mc->direction * mc->speed * (float)dt;
				debug("%llu at (%.2f, %.2f)", self, my_pos.x, my_pos.y);
				continue;
			}
		}

		glm::vec2 & tgt_pos = w.get(my_target)->get_component<position_component>()->position;
		if(tgt_pos != my_pos)
		{
			my_pos = glm::length(tgt_pos - my_pos) <= mc->speed*(float)dt
				? tgt_pos
				: my_pos + glm::normalize(tgt_pos - my_pos) * mc->speed * (float)dt;
			if(tgt_pos == my_pos)
			{
				if(my_state == movement_component::SEEK_MATE) {
					debug("%llu mating with %llu", my_id, my_target);
					rc->state = reproduction_component::MATING;
				} else if(my_state == movement_component::SEEK_FOOD) {
					debug("%llu eating %llu", my_id, my_target);
					hc->state = health_component::EATING;
				}
			}
		}
	}
	return true;
}
