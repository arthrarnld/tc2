#ifndef WORLD_HPP
#define WORLD_HPP

#include <cstdint>
#include <cstddef>
#include <list>
#include "components.hpp"

struct world
{
	world();
	uint64_t create();
	void update(double dt);

	inline size_t count() const
		{ return m_count; }

	movement		mov;
	position		pos;
	reproduction	rep;
	health			hea;
	species			spe;

	void print();

private:
	std::list<uint64_t> m_freelist;
	uint64_t m_next_index;
	size_t m_count;
};

#endif // WORLD_HPP
