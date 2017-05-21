#ifndef BASE_SYSTEM_HPP
#define BASE_SYSTEM_HPP

#include "../world.hpp"

class base_system
{
public:
	virtual bool handle_message(base_message * m, world & w) { return false; }

	// using iterator_type = world::iterator_type;
	virtual bool update(world & w, double dt) = 0;
protected:
	base_system() = default;
};

#endif // BASE_SYSTEM_HPP
