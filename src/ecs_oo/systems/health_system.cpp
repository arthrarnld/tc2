#include "systems/health_system.hpp"

#include "common/log.hpp"
#include "world.hpp"
#include "components/health_component.hpp"
#include "components/movement_component.hpp"


bool health_system::update(world & w, double dt)
{
	for(auto & e : w)
	{
		if(!e->has_component<health_component>())
			continue;

		auto & hc = *(e->get_component<health_component>());
		if(hc.state == health_component::EATING) {
			hc.hunger -= 25.0f;
			if(hc.hunger <= 0) {
				hc.hunger = 0;
				hc.state = health_component::IDLE;
				auto & mc = *(e->get_component<movement_component>());
				mc.state = movement_component::IDLE;
				mc.target.reset();
				debug("%llu done eating", e->get_id());
			}
		}
		else
			hc.hunger += hc.appetite * dt;
	}
	return true;
}
