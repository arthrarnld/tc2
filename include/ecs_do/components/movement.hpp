#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "common/util.hpp"
#include "../soa_utils.hpp"

struct movement
{
	movement(size_t c = 8)
		: helper(this, c,
			&target,
			&speed
			#ifndef DO_PARTITION_ARRAYS
				, &state
			#endif
		)
	{  }

	#ifdef DO_PARTITION_ARRAYS
		SOA_PARTITIONED_COMPONENT_BASE(movement)
	#else
		SOA_COMPONENT_BASE(movement)
	#endif

	inline size_t create(uint64_t e, float s) {
		size_t i = create(e);
		speed[i] = s;
		target[i] = nil;

		#ifdef DO_PARTITION_ARRAYS
			i = move(i, 0, MEMBER_SWAP_FUNC, partitions);
			// print_partitioned(owner, size(), partitions);
		#else
			state[i] = IDLE;
		#endif

		return i;
	}


	inline size_t seek_food(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			bool moved = move(i, 1, MEMBER_SWAP_FUNC, partitions) != i;
			// print_partitioned(owner, size(), partitions);
			if(moved)
				return i;
			return i+1;
		#else
			state[i] = SEEK_FOOD;
			return i + 1;
		#endif
	}

	inline size_t stop_seeking_food(size_t i)
	{
		target[i] = nil;
		#ifdef DO_PARTITION_ARRAYS
			move(i, 0, MEMBER_SWAP_FUNC, partitions);
			// print_partitioned(owner, size(), partitions);
		#else
			state[i] = IDLE;
		#endif

		return i + 1;
	}

	inline size_t seek_mate(size_t i)
	{
		#ifdef DO_PARTITION_ARRAYS
			bool moved = move(i, 2, MEMBER_SWAP_FUNC, partitions) != i;
			// print_partitioned(owner, size(), partitions);
			if(moved)
				return i;
			return i+1;
		#else
			state[i] = SEEK_MATE;
			return i + 1;
		#endif
	}

	inline size_t stop_seeking_mate(size_t i)
	{
		target[i] = nil;
		#ifdef DO_PARTITION_ARRAYS
			move(i, 0, MEMBER_SWAP_FUNC, partitions);
			// print_partitioned(owner, size(), partitions);
		#else
			state[i] = IDLE;
		#endif

		return i + 1;
	}

	inline void print()
	{
		fprintf(stderr, "movement: { ");
		for(size_t i = 0; i < len; ++i) {
			fprintf(stderr, "%llu: %llu", owner[i], target[i]);
			if(i < len-1)
				fprintf(stderr, ", ");
		}
		fprintf(stderr, " }\n");
	}

	uint64_t * target;
	float * speed;

	#ifdef DO_PARTITION_ARRAYS
		// |-----0-----|-----1-----|-----2-----|
		// |---idle----|---food----|---mate----|
		// ^0          ^p[0]       ^p[1]       ^size
		size_t partitions[2] = { 0, 0 };
	#else
		enum state_type { IDLE, SEEK_FOOD, SEEK_MATE };
		state_type * state;
	#endif
};

#endif // MOVEMENT_COMPONENT_HPP
