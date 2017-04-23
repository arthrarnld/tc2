#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>
#include <memory>
#include <functional>

#include "entity.hpp"
#include "messages/base_message.hpp"

class base_system;

class world
{
public:
	using entity_ptr = std::shared_ptr<entity>;
	using system_ptr = std::shared_ptr<base_system>;
	using message_ptr = std::shared_ptr<base_message>;
	using container_type = std::unordered_map<entity::id, entity_ptr>;
	using iterator_type = container_type::iterator;

	bool update(double dt);

	entity & create();
	void create(int count);
	entity_ptr get(entity::id id);
	void destroy(entity & e);
	unsigned get_entity_count();

	void push_system(base_system * s);

	void push_message(base_message * m);

	container_type::iterator begin();
	container_type::iterator end();

	void for_each(std::function<void(entity&)> && fn);

private:
	container_type m_entities;
	std::vector<system_ptr> m_systems;
	std::vector<message_ptr> m_messages;

	std::list<entity::id> m_freelist;
	entity::id m_counter = 0;

	void flush_messages();
};

#endif // WORLD_HPP
