#ifndef BASE_SYSTEM_HPP
#define BASE_SYSTEM_HPP

#include "../world.hpp"

class base_system
{
public:
	using iterator_type = world::iterator_type;
	virtual bool update(iterator_type begin, iterator_type end, double dt) = 0;
protected:
	base_system() = default;
};

#endif // BASE_SYSTEM_HPP
