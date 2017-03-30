#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdint>

class base_component;

class message
{
public:
	virtual uint64_t get_id() = 0;

protected:
	message(base_component * sender)
		: m_sender(sender)
	{  }
	virtual ~message() = default;
private:
	base_component * m_sender;
};

#endif // MESSAGE_HPP
