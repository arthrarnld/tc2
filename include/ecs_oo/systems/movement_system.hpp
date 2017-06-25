#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

class world;

class movement_system
{
public:
	bool update(world & w, double dt);
};

#endif // MOVEMENT_SYSTEM_HPP
