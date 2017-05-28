#ifndef HEALTH_SYSTEM_HPP
#define HEALTH_SYSTEM_HPP

#include "base_system.hpp"
#include "../components/health_component.hpp"

class health_system : public base_system
{
public:
	health_system() = default;

	bool update(world & w, double dt) override;
};

#endif // HEALTH_SYSTEM_HPP
