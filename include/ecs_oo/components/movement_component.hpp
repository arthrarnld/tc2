#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "base_component.hpp"

class movement_component : public base_component
{
public:
	movement_component(uint64_t owner) : base_component(owner) { }

	uint64_t target;
};

#endif // MOVEMENT_COMPONENT_HPP
