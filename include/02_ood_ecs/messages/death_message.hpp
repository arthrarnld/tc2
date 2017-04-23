#ifndef DEATH_MESSAGE_HPP
#define DEATH_MESSAGE_HPP

#include "base_message.hpp"

class death_message : public base_message
{
public:
	static const id_type id;

	id_type get_id() override
	{
		return id;
	}

	death_message(uint64_t sender)
		: base_message(sender)
	{  }
};

#endif // DEATH_MESSAGE_HPP
