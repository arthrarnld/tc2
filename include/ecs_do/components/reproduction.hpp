#ifndef REPRODUCTION_COMPONENT_HPP
#define REPRODUCTION_COMPONENT_HPP

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

	SOA_COMPONENT_BASE(reproduction)

	inline size_t create(uint64_t e, float lib) {
		size_t idx = create(e);
		desire[idx] = 0;
		libido[idx] = lib;

		#ifdef DO_PARTITION_ARRAYS
			first_mating = idx + 1;
		#else
			state[idx] = IDLE;
		#endif

		return idx;
	}

	inline size_t start_mating(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			--first_mating;
			if(first_mating > i) {
				swap(i, first_mating);
				return i;
			}
			return i + 1;
		#else
			state[i] = MATING;
			return i + 1;
		#endif
	}

	inline size_t stop_mating(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			++first_mating;
			if(first_mating <= i)
				swap(i, first_mating-1);
		#else
			state[i] = IDLE;
		#endif

		return i + 1;
	}

	// Properties:
	float * desire;
	float * libido;

	#ifdef DO_PARTITION_ARRAYS
		size_t first_mating;
	#else
		enum state_type { IDLE, MATING };
		state_type * state;
	#endif
};

#endif // REPRODUCTION_COMPONENT_HPP
