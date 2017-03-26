#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP

#include <vector>

#include "message.hpp"

class base_component
{
public:
	virtual bool handle_message(message *) { return false; }
protected:
	base_component() = default;
};

#endif // BASE_COMPONENT_HPP
