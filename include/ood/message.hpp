#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdint>

class message
{
public:
	virtual uint64_t get_id() = 0;

protected:
	message() = default;
	virtual ~message() = default;
};

#endif // MESSAGE_HPP
