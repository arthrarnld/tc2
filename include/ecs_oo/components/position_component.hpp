#ifndef POSITION_COMPONENT_HPP
#define POSITION_COMPONENT_HPP

////////////////////////////////////////////////////////////////////////////////
// Components

#include "base_component.hpp"

#include "glm/glm.hpp"

class position_component : public base_component
{
public:
	position_component(uint64_t owner) : base_component(owner) { }
	position_component(uint64_t owner, float x, float y)
		: base_component(owner)
		, position(x,y)
	{

	}

	glm::vec2 position;
};

#endif // POSITION_COMPONENT_HPP
