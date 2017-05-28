#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <cstdint>
#include <unordered_map>


#include "../soa_utils.hpp"

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

	SOA_COMPONENT_BASE(health)

	inline size_t create(uint64_t e, float apt, int pr) {
		size_t idx = create(e);
		hunger[idx] = 100;
		appetite[idx] = apt;
		prey[idx] = pr;

		#ifdef DO_PARTITION_ARRAYS
			first_eating = idx + 1;
		#else
			state[idx] = IDLE;
		#endif

		return idx;
	}

	inline size_t start_eating(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			--first_eating;
			if(first_eating > i) {
				swap(i, first_eating);
				return i;
			}
			return i + 1;
		#else
			state[i] = EATING;
			return i + 1;
		#endif
	}

	inline size_t stop_eating(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			++first_eating;
			if(first_eating <= i)
				swap(i, first_eating-1);
		#else
			state[i] = IDLE;
		#endif

		return i + 1;
	}


	float * hunger;
	float * appetite;
	int   * prey;

	#ifdef DO_PARTITION_ARRAYS
		size_t first_eating;
	#else
		enum state_type { IDLE, EATING };
		state_type * state;
	#endif

};

#endif // HEALTH_COMPONENT_HPP
