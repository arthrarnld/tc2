#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <algorithm>

#include "base_component.hpp"
#include "../entity.hpp"

class health_component : public base_component
{
public:
	health_component(uint64_t owner, float max, float regen)
		: base_component(owner)
		, health(max)
		, max_health(max)
		, regen_rate(regen)
	{

	}

	float health;
	float max_health;
	float regen_rate;
};

#endif // HEALTH_COMPONENT_HPP
