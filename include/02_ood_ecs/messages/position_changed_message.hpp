#ifndef POSITION_CHANGED_MESSAGE_HPP
#define POSITION_CHANGED_MESSAGE_HPP

#include "base_message.hpp"

class position_changed_message : public base_message
{
public:
	static const id_type id;

	position_changed_message(uint64_t sender)
		: base_message(sender)
	{  }

	uint64_t get_id()
	{
		return id_gen::get<position_changed_message>();
	}
};

#endif // POSITION_CHANGED_MESSAGE_HPP
