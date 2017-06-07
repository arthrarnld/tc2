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

	inline void print()
	{
		fprintf(stderr, "species: { ");
		for(size_t i = 0; i < len; ++i) {
			fprintf(stderr, "%llu: %d", owner[i], id[i]);
			if(i < len-1)
				fprintf(stderr, ", ");
		}
		fprintf(stderr, " }\n");
	}

	int * id;
};

#endif // SPECIES_COMPONENT_HPP
