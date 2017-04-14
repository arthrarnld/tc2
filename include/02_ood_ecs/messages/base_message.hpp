#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdint>

#include "../id_generator.hpp"

class base_message
{
public:
	using id_type = uint64_t;
	using id_gen = id_generator<base_message>;

	virtual ~base_message() = default;

	virtual id_type get_id() = 0;
	uint64_t get_sender() { return m_sender; }

protected:
	base_message(uint64_t sender)
		: m_sender(sender)
		{  }
private:
	uint64_t m_sender;
};

#endif // MESSAGE_HPP
