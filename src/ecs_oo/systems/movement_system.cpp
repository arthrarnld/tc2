#include "systems/movement_system.hpp"

#include "common/log.hpp"

#include "components/movement_component.hpp"
#include "components/position_component.hpp"
#include "components/species_component.hpp"
#include "components/reproduction_component.hpp"
#include "components/health_component.hpp"

static const float inf = std::numeric_limits<float>::infinity();

entity::id find_closest_of(world & w, const glm::vec2 & my_pos, int target_species)
{
	float dist = inf;
	float d;
	entity::id curr = entity::NIL;
	for(auto & p : w)
	{
		auto e = p.second;
		auto sc = e->get_component<species_component>();
		if(sc->id == target_species)
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
		auto e = p.second;

		// Movement
		auto mc = e->get_component<movement_component>();
		uint64_t & my_target = mc->target;
		movement_component::state_type my_state = mc->state;
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
			if(rc->desire > 50)
			{
				my_target = find_closest_of(w, my_pos, my_species);
				if(my_target != nil)
					my_state = movement_component::SEEK_MATE;
			}
			if(my_target == nil && hc->hunger > 50)
			{
				my_target = find_closest_of(w, my_pos, hc->prey);
				if(my_target != nil)
					my_state = movement_component::SEEK_FOOD;
			}
			if(my_target == nil)
				continue;
		}

		glm::vec2 & tgt_pos = w.get(my_target)->get_component<position_component>()->position;
		if(tgt_pos == my_pos)
		{
			if(my_state == movement_component::SEEK_MATE) {
				rc->state = reproduction_component::MATING;
			} else if(my_state == movement_component::SEEK_FOOD) {
				hc->state = health_component::EATING;
			}
		}
		else
			my_pos += glm::normalize(tgt_pos - my_pos) * mc->speed * (float)dt;
		debug("[%llu] at (%.2f,%.2f)", e->get_id(), my_pos.x, my_pos.y);
	}
	return true;
}
