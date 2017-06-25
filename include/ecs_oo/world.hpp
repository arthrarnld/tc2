#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>
#include <memory>
#include <functional>

#include "entity.hpp"
#include "systems.hpp"

class base_system;

class world
{
public:
	using container_type = std::vector< std::shared_ptr<entity> >;
	using iterator = container_type::iterator;

	void update(double dt);

	entity & create();
	void create(int count);
	// entity_ptr get(entity::id id);
	unsigned get_entity_count();

	iterator begin();
	iterator end();

private:
	container_type m_entities;

	health_system health;
	reproduction_system reproduction;
	movement_system movement;
};

#endif // WORLD_HPP
