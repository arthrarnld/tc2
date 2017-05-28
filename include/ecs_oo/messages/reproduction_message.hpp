#ifndef REPRODUCTION_MESSAGE_HPP
#define REPRODUCTION_MESSAGE_HPP

#include "base_message.hpp"

class reproduction_message : public base_message
{
public:
	static const id_type id;

	id_type get_id() override
	{
		return id;
	}

	reproduction_message(uint64_t sender, uint64_t recipient)
		: base_message(sender, recipient)
	{  }
};

#endif // REPRODUCTION_MESSAGE_HPP
