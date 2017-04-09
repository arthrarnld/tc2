#ifndef HEALTH_SYSTEM_HPP
#define HEALTH_SYSTEM_HPP

#include "base_system.hpp"

class health_system : public base_system
{
public:
	health_system() = default;

	bool update(iterator_type begin, iterator_type end, double dt) override
	{
		for(iterator_type it = begin; it != end; ++it)
		{
			entity_ptr e = it->second;
			e->set_health(std::min(e->get_max_health(), e->get_health()+e->get_regen_rate()*dt));
		}
		return true;
	}
};

#endif // HEALTH_SYSTEM_HPP
