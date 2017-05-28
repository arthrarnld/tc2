#ifndef REPRODUCTION_COMPONENT_HPP
#define REPRODUCTION_COMPONENT_HPP

#include "base_component.hpp"

class reproduction_component : public base_component
{
public:
	enum state_type { IDLE, MATING };

	reproduction_component(uint64_t owner, float libido)
		: base_component(owner)
		, desire(0)
		, libido(libido)
		, state(IDLE)
	{

	}

	float desire;
	float libido;
	state_type state;
};

#endif // REPRODUCTION_COMPONENT_HPP
