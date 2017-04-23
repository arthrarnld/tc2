#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "base_system.hpp"

class movement_system : public base_system
{
public:
	bool update(world & w, double dt) override;
};

#endif // MOVEMENT_SYSTEM_HPP
