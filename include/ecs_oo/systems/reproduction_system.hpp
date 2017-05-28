#ifndef REPRODUCTION_SYSTEM_HPP
#define REPRODUCTION_SYSTEM_HPP

#include "base_system.hpp"

class reproduction_system : public base_system
{
public:
	reproduction_system() = default;

	bool update(world & w, double dt) override;
};

#endif // REPRODUCTION_SYSTEM_HPP
