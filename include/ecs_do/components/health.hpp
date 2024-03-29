#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <cstdint>
#include <unordered_map>


#include "../soa_utils.hpp"
#include "common/util.hpp"

struct health
{
	health(size_t cap = 8)
	{
		helper.init(
			this,
			cap,
			&hunger,
			&appetite,
			&prey
			#ifndef DO_PARTITION_ARRAYS
				, &state
			#endif
		);
	}

	#ifdef DO_PARTITION_ARRAYS
		SOA_PARTITIONED_COMPONENT_BASE(health)
	#else
		SOA_COMPONENT_BASE(health)
	#endif

	inline size_t create(uint64_t e, float apt, int pr) {
		size_t idx = create(e);
		hunger[idx] = random() % 100;
		appetite[idx] = apt;
		prey[idx] = pr;

		#ifdef DO_PARTITION_ARRAYS
			idx = move(idx, 0, MEMBER_SWAP_FUNC, partitions);
		#else
			state[idx] = IDLE;
		#endif

		return idx;
	}

	inline size_t start_eating(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			if(move(i, 1, MEMBER_SWAP_FUNC, partitions) != i)
				return i;
		#else
			state[i] = EATING;
		#endif

		return i + 1;
	}

	inline size_t stop_eating(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			if(move(i, 0, MEMBER_SWAP_FUNC, partitions) != i)
				return i;
		#else
			state[i] = IDLE;
		#endif

		return i + 1;
	}


	// inline void print()
	// {
	// 	fprintf(stderr, "\e[1mhealth\e[0m\n\tidle:");
	// 	for(size_t i = 0; i < partitions[0]; ++i)
	// 		fprintf(stderr, " %zu[%llu %.2f]", i, owner[i], hunger[i]);
	// 	fprintf(stderr, "\n\teating:");
	// 	for(size_t i = partitions[0]; i < len; ++i)
	// 		fprintf(stderr, " %zu[%llu %.2f]", i, owner[i], hunger[i]);
	// 	fprintf(stderr, "\n");
	// }


	float * hunger;
	float * appetite;
	int   * prey;

	#ifdef DO_PARTITION_ARRAYS
		size_t partitions[1];
	#else
		enum state_type { IDLE, EATING };
		state_type * state;
	#endif

};

#endif // HEALTH_COMPONENT_HPP
