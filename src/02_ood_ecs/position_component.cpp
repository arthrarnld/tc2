#include "position_component.hpp"

position_component::position_component(float x, float y)
	: m_position(x,y)
{

}

void position_component::translate(float x, float y)
{
	m_position.x += x;
	m_position.y += y;
}

void position_component::translate(const glm::vec2 & by)
{
	translate(by.x, by.y);
}


void position_component::set_position(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void position_component::set_position(const glm::vec2 & pos)
{
	m_position = pos;
}


glm::vec2 position_component::get_position()
{
	return m_position;
}
