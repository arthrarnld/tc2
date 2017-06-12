#include "systems/reproduction_system.hpp"

#include "common/log.hpp"

#include "world.hpp"
#include "components/reproduction_component.hpp"
#include "components/movement_component.hpp"

bool reproduction_system::update(world & w, double dt)
{
	for(auto & e : w)
	{
		if(!e->has_component<reproduction_component>())
			continue;

		auto & rc = *(e->get_component<reproduction_component>());
		if(rc.state == reproduction_component::MATING) {
			rc.desire -= 25.0f;
			if(rc.desire <= 0) {
				rc.desire = 0;
				rc.state = reproduction_component::IDLE;
				auto & mc = *(e->get_component<movement_component>());
				mc.state = movement_component::IDLE;
				mc.target.reset();
				debug("%llu done mating", e->get_id());
			}
		} else {
			rc.desire += rc.libido * dt;
		}
	}
	return true;
}
