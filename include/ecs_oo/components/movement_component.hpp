#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "base_component.hpp"
#include "common/constants.hpp"

class movement_component : public base_component
{
public:
	enum state_type { IDLE, SEEK_FOOD, SEEK_MATE };

	movement_component(uint64_t owner, float speed)
		: base_component(owner)
		, target(nil)
		, speed(speed)
		, state(IDLE)
	{

	}

	uint64_t target;
	float speed;
	state_type state;
};

#endif // MOVEMENT_COMPONENT_HPP
