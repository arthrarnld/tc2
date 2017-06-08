#ifndef SPECIES_COMPONENT_HPP
#define SPECIES_COMPONENT_HPP

#include "../soa_utils.hpp"

struct species
{
	species(size_t c = 8)
	{
		helper.init(this, c, &id);
	}

	SOA_COMPONENT_BASE(species)

	inline size_t create(uint64_t e, int i) {
		size_t idx = create(e);
		id[idx] = i;
		return idx;
	}

	// inline void print()
	// {
	// 	fprintf(stderr, "\e[1mspecies\e[0m:");
	// 	for(size_t i = 0; i < len; ++i)
	// 		fprintf(stderr, " %zu[%llu %d]", i, owner[i], id[i]);
	// 	fprintf(stderr, "\n");
	// }

	int * id;
};

#endif // SPECIES_COMPONENT_HPP
