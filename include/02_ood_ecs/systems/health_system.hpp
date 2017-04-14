#ifndef HEALTH_SYSTEM_HPP
#define HEALTH_SYSTEM_HPP

#include "base_system.hpp"
#include "../components/health_component.hpp"

class health_system : public base_system
{
public:
	health_system() = default;

	bool update(iterator_type begin, iterator_type end, double dt) override
	{
		for(iterator_type it = begin; it != end; ++it)
		{
			auto e = it->second;
			if(e->has_component<health_component>())
			{
				auto & hc = e->get_component<health_component>();
				hc.set_health(std::min((double)hc.get_max_health(), hc.get_health()+hc.get_regen_rate()*dt));
			}
		}
		return true;
	}
};

#endif // HEALTH_SYSTEM_HPP
