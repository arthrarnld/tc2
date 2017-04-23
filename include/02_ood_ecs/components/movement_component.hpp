#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "base_component.hpp"

class movement_component : public base_component
{
public:
	movement_component(uint64_t owner, float speed)
		: base_component(owner)
		, speed(speed)
	{
		
	}

	uint64_t target;
	float speed;
};

#endif // MOVEMENT_COMPONENT_HPP
