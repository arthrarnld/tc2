#include "systems/movement_system.hpp"

#include "common/log.hpp"

#include "components/movement_component.hpp"
#include "components/position_component.hpp"

bool movement_system::update(world & w, double dt)
{
	for(auto & p : w)
	{
		auto e = p.second;

		auto mc = e->get_component<movement_component>();
		if(!mc) continue;
		if(mc->target == entity::NIL) continue;

		auto pc = e->get_component<position_component>();
		if(!pc) continue;
		glm::vec2 & my_pos = pc->position;

		auto tgt = w.get(mc->target);
		if(!tgt)
			continue;

		auto tpc = tgt->get_component<position_component>();
		if(!tpc)
			continue;
		glm::vec2 & tgt_pos = tpc->position;

		my_pos += glm::normalize(tgt_pos - my_pos) * (mc->speed * (float)dt);
		debug("[%llu] at (%.2f,%.2f)", e->get_id(), my_pos.x, my_pos.y);
	}
	return true;
}
