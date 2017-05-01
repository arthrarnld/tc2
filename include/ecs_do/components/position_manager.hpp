#ifndef POSITION_MANAGER_HPP
#define POSITION_MANAGER_HPP

#include <stdint.h>
#include <unordered_map>
#include <glm/glm.hpp>

#include "../array.hpp"

struct position_manager
{
	unsigned create(uint64_t eid);
	void destroy(unsigned idx);

	std::unordered_map<uint64_t, unsigned> entity_to_index;
	array<uint64_t> index_to_entity;
	array<glm::vec2> positions;
};

#endif // POSITION_MANAGER_HPP
