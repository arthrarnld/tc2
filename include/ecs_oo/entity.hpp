#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include <list>
#include <cstdint>
#include <memory>

#include "common/log.hpp"

#include "components/base_component.hpp"
#include "components/movement_component.hpp"
#include "id_generator.hpp"

class world;

class entity
{
public:
	static constexpr uint64_t NIL = 0xffffffffffffffff;

	using id = uint64_t;
	using id_gen = id_generator<base_component>;
	static constexpr uint64_t VERSION_BITS = 8;
	static constexpr uint64_t INDEX_BITS = 8*sizeof(id) - VERSION_BITS;

	bool is_valid();

	id get_id();
	id get_index();
	id get_version();

	bool is_enabled();
	void disable();
	void enable();

	template <typename T>
	T * get_component()
	{
		assert_derived(T, base_component, "type must be a subclass of base_component");
		if(id_gen::get<T>() >= m_components.size())
			return nullptr;
		return static_cast<T*>(m_components[id_gen::get<T>()].get());
	}

	template <typename T>
	bool has_component()
	{
		assert_derived(T, base_component, "type must be a subclass of base_component");
		if(id_gen::get<T>() >= m_components.size())
			return false;
		return m_components[id_gen::get<T>()] != nullptr;
	}

	template<typename T, typename ... Args>
	T * add_component(Args&& ... args)
	{
		assert_derived(T, base_component, "type must be a subclass of base_component");
		T * comp = new T(m_id, args...);
		uint64_t pos = id_gen::get<T>();
		if(pos >= m_components.size())
			m_components.resize(pos+1);
		m_components[pos].reset(comp);
		return comp;
	}

	template<typename T>
	void remove_component()
	{
		assert_derived(T, base_component, "type must be a subclass of base_component");
		m_components[id_gen::get<T>()].reset(nullptr);
	}

	// void push_message(base_message * msg);

	~entity() = default;

	entity(const entity &) = delete;
	entity & operator=(const entity &) = delete;

private:
	friend class world;

	entity(id i);

	id m_id;
	bool m_alive;
	bool m_enabled;
	std::vector< std::shared_ptr<base_component> > m_components;
	// std::vector< std::shared_ptr<base_message> > m_messages;

	void flush_messages(world * w);
};

#endif // ENTITY_HPP
