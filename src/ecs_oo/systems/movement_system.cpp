#include "systems/movement_system.hpp"

#include "common/log.hpp"
#include "common/util.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "world.hpp"
#include "components/movement_component.hpp"
#include "components/position_component.hpp"
#include "components/species_component.hpp"
#include "components/reproduction_component.hpp"
#include "components/health_component.hpp"

static const float inf = std::numeric_limits<float>::infinity();

std::weak_ptr<entity> find_closest_of(world & w, std::weak_ptr<entity> self, const glm::vec2 & my_pos, int target_species)
{
	// static size_t called = 0;
	// fprintf(stderr, "%zu\t%p wants %d\n", ++called, self.lock().get(), target_species);
	float dist = inf;
	float d;
	std::weak_ptr<entity> curr;
	for(auto & e : w)
	{
		auto sc = e->get_component<species_component>();
		if(e != self.lock() && sc->id == target_species)
		{
			auto & their_pos = e->get_component<position_component>()->position;
			// fprintf(stderr, "\tpositions: (%.2f, %.2f), (%.2f, %.2f)\n",
				// my_pos.x, my_pos.y, their_pos.x, their_pos.y);
			d = glm::length(their_pos - my_pos);
			// fprintf(stderr, "\tfound target species %d at distance %.2f\n", target_species, d);
			if(d < dist)
			{
				// fprintf(stderr, "\t\t%p is closer\n", e.get());
				dist = d;
				curr = e;
			}
		}
	}
	return curr;
}

bool movement_system::update(world & w, double dt)
{
	// static size_t iteration = 0;

	for(auto & e : w)
	{
		if(!e->has_component<movement_component>())
			continue;

		// Movement
		auto mc = e->get_component<movement_component>();
		std::weak_ptr<entity> & my_target = mc->target;
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

		// fprintf(stderr, "[%zu] %p  state: %d  target: %p  hunger: %.2f  desire: %.2f  position: (%.2f, %.2f)\n",
		// 	iteration, e.get(), my_state, my_target.lock().get(), hc->hunger, rc->desire, my_pos.x, my_pos.y);

		if(my_state == movement_component::IDLE)
		{
			if(rc->desire > 50.0f && rc->desire > hc->hunger)
			{
				debug("[%.2f, %.2f] must mate", rc->desire, hc->hunger);
				my_target = find_closest_of(w, e, my_pos, my_species);
				if(my_target.lock()) {
					// fprintf(stderr, "\tfound mate %p\n", my_target.lock().get());
					my_state = movement_component::SEEK_MATE;
					debug("seeking to mate");
				}
			}
			if(!my_target.lock() && hc->hunger > 50.0f)
			{
				debug("must eat");
				my_target = find_closest_of(w, e, my_pos, hc->prey);
				if(my_target.lock()) {
					// fprintf(stderr, "\tfound food %p\n", my_target.lock().get());
					my_state = movement_component::SEEK_FOOD;
					debug("seeking to eat");
				}
			}
			if(!my_target.lock()) {
				// No target was found. Wander about randomly.
				glm::vec2 delta = mc->direction;
				float angle = drand(to_rad(-30), to_rad(30));
				delta = glm::rotate(delta, angle);
				mc->direction = glm::normalize(mc->direction + delta);
				// assert(my_pos.x == my_pos.x);
				// assert(my_pos.y == my_pos.y);

				my_pos += mc->direction * mc->speed * (float)dt;
				// assert(my_pos.x == my_pos.x);
				// assert(my_pos.y == my_pos.y);
				debug("(%.2f, %.2f)", my_pos.x, my_pos.y);
				continue;
			}
		}

		if(!my_target.lock()) // target no longer exists
		{
			my_target.reset();
			my_state = movement_component::IDLE;
		}

		glm::vec2 & tgt_pos = my_target.lock()->get_component<position_component>()->position;
		if(tgt_pos != my_pos)
		{
			// assert(my_pos.x == my_pos.x);
			// assert(my_pos.y == my_pos.y);

			my_pos = glm::length(tgt_pos - my_pos) <= mc->speed*(float)dt
				? tgt_pos
				: my_pos + glm::normalize(tgt_pos - my_pos) * mc->speed * (float)dt;

			// assert(my_pos.x == my_pos.x);
			// assert(my_pos.y == my_pos.y);
			if(tgt_pos == my_pos)
			{
				if(my_state == movement_component::SEEK_MATE) {
					// fprintf(stderr, "\tarrived at mate %p\n", my_target.lock().get());
					debug("%llu mating with %llu", my_id, my_target);
					rc->state = reproduction_component::MATING;
				} else if(my_state == movement_component::SEEK_FOOD) {
					// fprintf(stderr, "\tarrived at food %p\n", my_target.lock().get());
					debug("%llu eating %llu", my_id, my_target);
					hc->state = health_component::EATING;
				}
			}
		}
	}

	// ++iteration;
	return true;
}
