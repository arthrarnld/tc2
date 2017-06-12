#ifndef REPRODUCTION_COMPONENT_HPP
#define REPRODUCTION_COMPONENT_HPP

#include "base_component.hpp"

#include <cstdlib>

class reproduction_component : public base_component
{
public:
	enum state_type { IDLE, MATING };

	reproduction_component(float libido)
		: desire(random() % 40)
		, libido(libido)
		, state(IDLE)
	{

	}

	float desire;
	float libido;
	state_type state;
};

#endif // REPRODUCTION_COMPONENT_HPP
