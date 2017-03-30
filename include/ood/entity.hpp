#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include <list>
#include <cstdint>

#include "base_component.hpp"
#include "id_generator.hpp"
#include "message.hpp"

class entity
{
public:
	using id = uint64_t;
	using id_gen = id_generator<base_component>;
	static constexpr uint64_t VERSION_BITS = 8;
	static constexpr uint64_t INDEX_BITS = 8*sizeof(id) - VERSION_BITS;

	id get_id();
	id get_index();
	id get_version();

	bool is_enabled();
	void disable();
	void enable();

	template <typename T>
	T & get_component()
	{
		assert_derived(T, base_component, "type must be a subclass of base_component");
		return *m_components[id_gen::get<T>()];
	}

	void handle_message(message * msg);

	~entity() = default;

	entity(const entity &) = delete;
	entity & operator=(const entity &) = delete;

private:
	friend class world;
	friend class std::unordered_map<id,entity>;

	entity(id i);

	id m_id;
	bool m_enabled;
	std::vector<base_component *> m_components;
};

#endif // ENTITY_HPP
