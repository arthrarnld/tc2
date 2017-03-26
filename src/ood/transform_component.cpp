#include "ood/transform_component.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#define DECOMPOSE(M) \
	glm::vec3 scale,translation,__sk;\
	glm::quat __ori;\
	glm::vec4 __per;\
	glm::decompose(M, scale, __ori, translation, __sk, __per);\
	__ori = glm::conjugate(__ori);\
	glm::vec3 rotation = glm::eulerAngles(__ori);

transform_component::transform_component(float x, float y)
{
	translate(x,y);
}


void transform_component::translate(float x, float y)
{
	m_transform = glm::translate(m_transform, glm::vec3{x, y, 0});
}

void transform_component::translate(const glm::vec2 & d)
{
	translate(d.x, d.y);
}

void transform_component::set_position(float x, float y)
{
	translate(-get_position());
	translate(x, y);
}

glm::vec2 transform_component::get_position()
{
	DECOMPOSE(m_transform)
	return glm::vec2{ translation.x, translation.y };
}


void transform_component::rotate(float angle)
{
	static const glm::vec3 axis { 0, 0, 1 };
	m_transform = glm::rotate(m_transform, angle, axis);
}

void transform_component::set_rotation(float angle)
{
	rotate(-get_rotation());
	rotate(angle);
}

float transform_component::get_rotation()
{
	DECOMPOSE(m_transform)
	return rotation.z;
}


void transform_component::scale(const glm::vec2 & sc)
{
	m_transform = glm::scale(m_transform, glm::vec3{ sc, 0 });
}

void transform_component::set_scale(const glm::vec2 & sc)
{
	scale(-get_scale());
	scale(sc);
}

glm::vec2 transform_component::get_scale()
{
	DECOMPOSE(m_transform)
	return glm::vec2{ scale.x, scale.y };
}
