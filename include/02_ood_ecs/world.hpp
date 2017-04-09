#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>
#include <memory>

#include "entity.hpp"

class base_system;

class world
{
public:
	using entity_ptr = std::shared_ptr<entity>;
	using system_ptr = std::shared_ptr<base_system>;
	using container_type = std::unordered_map<entity::id, entity_ptr>;
	using iterator_type = container_type::iterator;

	bool update(double dt);

	entity_ptr create();
	void create(int count);
	entity_ptr get(entity::id id);
	void destroy(entity & e);

	template<typename S, typename ... Args>
	void push_system(Args && ... args)
	{ m_systems.emplace_back<S>(args...); }

private:
	container_type m_entities;
	std::vector<system_ptr> m_systems;

	std::list<entity::id> m_freelist;
	entity::id m_counter;
};

#endif // WORLD_HPP
