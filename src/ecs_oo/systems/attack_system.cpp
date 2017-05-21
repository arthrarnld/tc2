#include "systems/attack_system.hpp"

#include "common/log.hpp"

#include "components/position_component.hpp"
#include "components/team_component.hpp"
#include "components/movement_component.hpp"
#include "components/attack_component.hpp"
#include "messages/health_message.hpp"

static const float inf = std::numeric_limits<float>::infinity();

entity * find_target(world & w, uint64_t my_id, float & out_dist, const glm::vec2 & my_pos, int my_team)
{
	entity * target = nullptr;
	out_dist = inf;
	w.for_each([my_id, &target, &out_dist, &my_pos, my_team](entity & e) {
		if(e.get_id() == my_id)
			return;
		auto p = e.get_component<position_component>();
		if(!p)
			return;
		auto their_tc = e.get_component<team_component>();
		int their_team = their_tc ? their_tc->team : -1;
		if(my_team >= 0 && their_team == my_team) // same team: don't attack
			return;
		float d = glm::length(p->position - my_pos);
		if(d < out_dist) {
			out_dist = d;
			target = &e;
		}
	});
	return target;
}

bool attack_system::update(world & w, double dt)
{
	for(auto & pair : w)
	{
		auto e = pair.second;
		// Get attack component
		auto ac = e->get_component<attack_component>();
		if(!ac)
			continue;
		if(ac->reload_rem > 0)
			ac->reload_rem -= dt;

		if(ac->reload_rem <= 0)
			ac->reload_rem = 0;
		else	// weapon reloading; cannot attack
			continue;

		// Get position component
		auto pc = e->get_component<position_component>();
		if(!pc)
			continue;

		// Get team component (optional)
		auto tc = e->get_component<team_component>();
		int team = tc ? tc->team : -1;

		// Get the movement component (optional)
		auto mc = e->get_component<movement_component>();

		float distance;
		entity * target = find_target(w, e->get_id(), distance, pc->position, team);
		if(target) {
			if(distance <= ac->range)
			{
				debug("%llu attacking %llu", e->get_id(), target->get_id());
				ac->state = attack_component::ENGAGED;
				ac->reload_rem = ac->reload;
				w.push_message(new health_message(
					e->get_id(),		// from
					target->get_id(),	// to
					ac->damage			// damage
				));
				if(mc && mc->target != entity::NIL) // cancel movement
					mc->target = entity::NIL;
			}
			else if(mc) {
				debug("%llu pursuing %llu", e->get_id(), target->get_id());
				mc->target = target->get_id();
			}
		} else {
			ac->state = attack_component::IDLE;
		}
	}
	return true;
}
