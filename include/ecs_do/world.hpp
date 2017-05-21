#ifndef WORLD_HPP
#define WORLD_HPP

#include <cstdint>
#include <list>

struct world
{
	uint64_t create();
	void destroy(uint64_t eid);

private:
	std::list<uint64_t> m_freelist;
	uint64_t m_next_index;
};

#endif // WORLD_HPP
