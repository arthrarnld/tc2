#ifndef HEALTH_MESSAGE_HPP
#define HEALTH_MESSAGE_HPP

#include "base_message.hpp"

class health_message : public base_message
{
public:
	static const id_type id;

	id_type get_id() override
	{
		return id;
	}

	health_message(uint64_t sender, float value)
		: base_message(sender)
		, m_value(value)
		{  }

	float get_value() { return m_value; }
	void set_value(float v) { m_value = v; }

private:
	float m_value;
};

#endif // HEALTH_MESSAGE_HPP
