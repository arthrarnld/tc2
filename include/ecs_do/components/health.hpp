#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <cstdint>
#include <unordered_map>


#include "../soa_utils.hpp"
#include "common/util.hpp"

struct health
{
	health(size_t cap = 8)
		: helper(
			this,
			cap,
			&hunger,
			&appetite,
			&prey
			#ifndef DO_PARTITION_ARRAYS
				, &state
			#endif
		)

	{  }

	#ifdef DO_PARTITION_ARRAYS
		SOA_PARTITIONED_COMPONENT_BASE(health)
	#else
		SOA_COMPONENT_BASE(health)
	#endif

	inline size_t create(uint64_t e, float apt, int pr) {
		size_t idx = create(e);
		hunger[idx] = 100;
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
			return i + 1;
		#else
			state[i] = EATING;
			return i + 1;
		#endif
	}

	inline size_t stop_eating(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			if(move(i, 0, MEMBER_SWAP_FUNC, partitions) != i)
				return i;
				return i + 1;
		#else
			state[i] = IDLE;
		#endif

		return i + 1;
	}

	inline void print()
	{
		#ifdef DO_PARTITION_ARRAYS
			fprintf(stderr, "health: { 0 | %zu | %zu }\n", partitions[0], size());
		#else
			fprintf(stderr, "health: { 0 | %zu }\n", size());
		#endif
	}


	float * hunger;
	float * appetite;
	int   * prey;

	#ifdef DO_PARTITION_ARRAYS
		size_t partitions[1] = { 0 };
	#else
		enum state_type { IDLE, EATING };
		state_type * state;
	#endif

};

#endif // HEALTH_COMPONENT_HPP
