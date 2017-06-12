#ifndef POSITION_COMPONENT_HPP
#define POSITION_COMPONENT_HPP

////////////////////////////////////////////////////////////////////////////////
// Components

#include "base_component.hpp"

#include "glm/glm.hpp"

class position_component : public base_component
{
public:
	position_component() = default;
	position_component(float x, float y)
		: position(x,y)
	{

	}

	glm::vec2 position;
};

#endif // POSITION_COMPONENT_HPP
