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
	using id_gen = id_generator<base_component>;

	~entity() = default;

	entity(const entity &) = delete;
	entity & operator=(const entity &) = delete;

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
		T * comp = new T(args...);
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

private:
	entity() = default;
	friend class world;
	
	std::vector< std::shared_ptr<base_component> > m_components;
};

#endif // ENTITY_HPP
