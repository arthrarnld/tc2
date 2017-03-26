#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "base_component.hpp"

#include "glm/glm.hpp"

class transform_component : public base_component
{
public:
	transform_component() = default;
	transform_component(float x, float y);

	void translate(float x, float y);
	void translate(const glm::vec2 & d);
	void set_position(float x, float y);
	glm::vec2 get_position();

	void rotate(float angle);
	void set_rotation(float angle);
	float get_rotation();

	void scale(const glm::vec2 & scale);
	void set_scale(const glm::vec2 & scale);
	glm::vec2 get_scale();

private:
	glm::mat4 m_transform;
};

#endif // TRANSFORM_COMPONENT_HPP
