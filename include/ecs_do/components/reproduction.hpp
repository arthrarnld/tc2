#ifndef REPRODUCTION_COMPONENT_HPP
#define REPRODUCTION_COMPONENT_HPP

#include "common/util.hpp"
#include "../soa_utils.hpp"

struct reproduction
{
	reproduction(size_t c = 8)
		: helper(this,
			c,
			&desire,
			&libido
			#ifndef DO_PARTITION_ARRAYS
				, &state
			#endif
		)
	{  }

	#ifdef DO_PARTITION_ARRAYS
		SOA_PARTITIONED_COMPONENT_BASE(reproduction)
	#else
		SOA_COMPONENT_BASE(reproduction)
	#endif

	inline size_t create(uint64_t e, float lib) {
		size_t idx = create(e);
		desire[idx] = random() % 40;
		libido[idx] = lib;

		#ifdef DO_PARTITION_ARRAYS
			idx = move(idx, 0, MEMBER_SWAP_FUNC, partitions);
		#else
			state[idx] = IDLE;
		#endif

		return idx;
	}

	inline size_t start_mating(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			if(move(i, 1, MEMBER_SWAP_FUNC, partitions) != i)
				return i;
			return i + 1;
		#else
			state[i] = MATING;
			return i + 1;
		#endif
	}

	inline size_t stop_mating(size_t i)
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
		fprintf(stderr, "reproduction: { ");
		for(size_t i = 0; i < len; ++i) {
			fprintf(stderr, "%llu: %.2f", owner[i], desire[i]);
			if(i < len-1)
				fprintf(stderr, ", ");
		}
		fprintf(stderr, " }\n");
	}

	// Properties:
	float * desire;
	float * libido;

	#ifdef DO_PARTITION_ARRAYS
		size_t partitions[1];
	#else
		enum state_type { IDLE, MATING };
		state_type * state;
	#endif
};

#endif // REPRODUCTION_COMPONENT_HPP
