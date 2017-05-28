#ifndef SPECIES_COMPONENT_HPP
#define SPECIES_COMPONENT_HPP

#include "../soa_utils.hpp"

struct species
{
	species(size_t c = 8)
		: helper(this, c, &id)
	{  }

	SOA_COMPONENT_BASE(species)

	inline size_t create(uint64_t e, int i) {
		size_t idx = create(e);
		id[idx] = i;
		return idx;
	}

	int * id;
};

#endif // SPECIES_COMPONENT_HPP
