#include "systems/reproduction_system.hpp"

#include "common/log.hpp"

#include "components/reproduction_component.hpp"

bool reproduction_system::update(world & w, double dt)
{
	for(auto & pair : w)
	{
		auto e = pair.second;
		auto rc = e->get_component<reproduction_component>();
		if(rc->state == reproduction_component::MATING)
			--rc->desire;
		else
			rc->desire += rc->libido * dt;
	}
	return true;
}
