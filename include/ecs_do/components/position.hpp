#ifndef POSITION_COMPONENT_HPP
#define POSITION_COMPONENT_HPP

#include <stdint.h>
#include <unordered_map>
#include <glm/glm.hpp>

#include "../soa_utils.hpp"

struct position
{
	position(size_t c = 8)
		: helper(this, c, &pos)
	{  }

	SOA_COMPONENT_BASE(position)

	inline size_t create(uint64_t e, float x, float y) {
		size_t i = create(e);
		pos[i] = glm::vec2{x,y};
		return i;
	}

	inline void print()
	{
		fprintf(stderr, "position: { ");
		for(size_t i = 0; i < len; ++i) {
			fprintf(stderr, "%llu: (%.2f,%.2f)", owner[i], pos[i].x, pos[i].y);
			if(i < len-1)
				fprintf(stderr, ", ");
		}
		fprintf(stderr, " }\n");
	}


	glm::vec2 * pos;
};


#endif // POSITION_COMPONENT_HPP
