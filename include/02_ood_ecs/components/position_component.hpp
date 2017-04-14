#ifndef POSITION_COMPONENT_HPP
#define POSITION_COMPONENT_HPP

////////////////////////////////////////////////////////////////////////////////
// Components

#include "base_component.hpp"

#include "glm/glm.hpp"

class position_component : public base_component
{
public:
	position_component(uint64_t owner);
	position_component(uint64_t owner, float x, float y);

	void translate(float x, float y);
	void translate(const glm::vec2 & by);

	void set_position(float x, float y);
	void set_position(const glm::vec2 & pos);

	glm::vec2 get_position();


private:
	glm::vec2 m_position;
};

#endif // POSITION_COMPONENT_HPP
