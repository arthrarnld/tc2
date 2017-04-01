#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>
#include <memory>

#include "entity.hpp"

class world
{
public:
	using pointer_type = std::shared_ptr<entity>;

	pointer_type create();
	void create(int count);
	pointer_type get(entity::id id);
	void destroy(entity & e);

private:
	std::unordered_map<entity::id, pointer_type> m_entities;
	std::list<entity::id> m_freelist;
	entity::id m_counter;
};

#endif // WORLD_HPP
