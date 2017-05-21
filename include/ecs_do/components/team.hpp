#ifndef TEAM_COMPONENT_HPP
#define TEAM_COMPONENT_HPP

#include "../soa_utils.hpp"

struct team
{
	team(size_t c = 8)
		: helper(this, c, &team_id)
	{  }

	SOA_COMPONENT_BASE(team)

	inline size_t create(uint64_t e, int id) {
		size_t i = create(e);
		team_id[i] = id;
		return i;
	}

	int * team_id;
};

#endif // TEAM_COMPONENT_HPP
