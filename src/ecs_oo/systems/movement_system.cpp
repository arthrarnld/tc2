#include "systems/movement_system.hpp"

#include "components/movement_component.hpp"
#include "components/attack_component.hpp"

bool movement_system::update(world & w, double dt)
{
	for(auto & p : w)
	{
		auto e = p.second;
		auto mc = e->get_component<movement_component>();
		if(!mc)
			continue;
		if(mc->target == entity::NIL)
			continue;
		
	}
}
