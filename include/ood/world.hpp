#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>

#include "entity.hpp"

class world
{
public:
	entity & create(int count = 1);
	void destroy(entity & e);

private:
	std::unordered_map<entity::id,entity> m_entities;
	std::list<entity::id> m_freelist;
	entity::id m_counter;
};

#endif // WORLD_HPP