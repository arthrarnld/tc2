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
	position_component(float x, float y);

	void translate(float x, float y);
	void translate(const glm::vec2 & by);

	void set_position(float x, float y);
	void set_position(const glm::vec2 & pos);

	glm::vec2 get_position();


private:
	glm::vec2 m_position;
};


////////////////////////////////////////////////////////////////////////////////
// Messages

#include "message.hpp"
#include "id_generator.hpp"

class position_changed : public message
{
public:
	position_changed(position_component * sender)
		: message(sender)
	{  }

	uint64_t get_id()
	{
		return id_generator<message>::get<position_changed>();
	}
};

#endif // POSITION_COMPONENT_HPP
