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

	health_message(uint64_t sender, uint64_t recipient, float value)
		: base_message(sender, recipient)
		, value(value)
		{  }

	float value;
};

#endif // HEALTH_MESSAGE_HPP
