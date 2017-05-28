#include "systems/health_system.hpp"

#include "common/log.hpp"

#include "messages/health_message.hpp"

bool health_system::update(world & w, double dt)
{
	for(auto & pair : w)
	{
		auto e = pair.second;
		if(e->has_component<health_component>())
		{
			auto & hc = *(e->get_component<health_component>());
			if(hc.state == health_component::EATING) {
				--hc.hunger;
				if(hc.hunger <= 0) {
					hc.hunger = 0;
					hc.state = health_component::EATING;
				}
			}
			else
				hc.hunger += hc.appetite * dt;
		}
	}
	return true;
}
