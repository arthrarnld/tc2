#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <algorithm>

#include "base_component.hpp"
#include "../entity.hpp"

class health_component : public base_component
{
public:
	enum state_type { IDLE, EATING };

	health_component(float appetite, int prey)
		: hunger(random() % 100)
		, appetite(appetite)
		, prey(prey)
		, state(IDLE)
	{

	}

	float hunger;
	float appetite;
	int prey;
	state_type state;
};

#endif // HEALTH_COMPONENT_HPP
