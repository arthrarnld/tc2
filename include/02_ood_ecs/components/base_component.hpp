#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP

#include "../messages/base_message.hpp"

class world;

class base_component
{
public:
	virtual bool handle_message(base_message * m, world * w) { return false; }

protected:
	base_component(uint64_t owner) : m_owner(owner) {  }
	uint64_t m_owner;
};

#endif // BASE_COMPONENT_HPP
