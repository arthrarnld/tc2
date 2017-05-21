#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP

#include <type_traits>
#include <cstdint>

#define assert_derived(T, B, msg) static_assert(\
		std::is_base_of<B, T>::value\
		&& !std::is_same<B, T>::value,\
		msg);

template <typename Base>
class id_generator
{
public:

	template <typename T>
	static uint64_t get()
	{
		assert_derived(T, Base, "type must be a subclass of base");
		static uint64_t id = m_counter++;
		return id;
	}

	static uint64_t count()
	{
		return m_counter;
	}

private:
	static uint64_t m_counter;
};

template <typename Base>
uint64_t id_generator<Base>::m_counter = 0;

#endif // ID_GENERATOR_HPP
