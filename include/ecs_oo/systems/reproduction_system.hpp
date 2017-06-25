#ifndef REPRODUCTION_SYSTEM_HPP
#define REPRODUCTION_SYSTEM_HPP

class world;

class reproduction_system
{
public:
	bool update(world & w, double dt);
};

#endif // REPRODUCTION_SYSTEM_HPP
