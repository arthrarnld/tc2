#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdint>

#include "../id_generator.hpp"
#include "../entity.hpp"

class base_message
{
public:
	using id_type = uint64_t;
	using id_gen = id_generator<base_message>;

	virtual ~base_message() = default;

	virtual id_type get_id() = 0;

protected:
	base_message(uint64_t sender, uint64_t recipient = entity::NIL)
		: sender(sender)
		, recipient(recipient)
		{  }

public:
	uint64_t sender;
	uint64_t recipient;
};

#endif // MESSAGE_HPP
