#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "base_component.hpp"
#include "common/constants.hpp"
#include "common/util.hpp"
#include "glm/glm.hpp"

class entity;

class movement_component : public base_component
{
public:
	enum state_type { IDLE, SEEK_FOOD, SEEK_MATE };

	movement_component(float speed)
		: target()
		, speed(speed)
		, state(IDLE)
	{
		double x = drand(0,1);
		direction = glm::vec2 { x, sqrt(1 - x*x)*(irand(0, 1) ? 1 : -1) };
	}

	std::weak_ptr<entity> target;
	float speed;
	glm::vec2 direction;
	state_type state;
};

#endif // MOVEMENT_COMPONENT_HPP
