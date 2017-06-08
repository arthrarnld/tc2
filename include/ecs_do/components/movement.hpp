#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "common/util.hpp"
#include "../soa_utils.hpp"
#include "glm/glm.hpp"

struct movement
{
	movement(size_t c = 8)
	{
		helper.init(
			this,
			c,
			&target,
			&speed,
			&direction
			#ifndef DO_PARTITION_ARRAYS
				, &state
			#endif
		);
	}

	#ifdef DO_PARTITION_ARRAYS
		SOA_PARTITIONED_COMPONENT_BASE(movement)
	#else
		SOA_COMPONENT_BASE(movement)
	#endif

	inline size_t create(uint64_t e, float s) {
		size_t i = create(e);
		speed[i] = s;
		target[i] = nil;
		// Make a random unit vector
		float x = drand(0,1);
		direction[i] = glm::vec2{x, sqrt(1 - x*x)*(irand(0, 1) ? 1 : -1)};

		#ifdef DO_PARTITION_ARRAYS
			i = move(i, 0, MEMBER_SWAP_FUNC, partitions);
		#else
			state[i] = IDLE;
		#endif

		return i;
	}


	inline size_t seek_food(size_t i)
	{
		A(i < len);
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
		A(i < len);
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
		A(i < len);
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
		A(i < len);
		target[i] = nil;
		#ifdef DO_PARTITION_ARRAYS
			move(i, 0, MEMBER_SWAP_FUNC, partitions);
			// print_partitioned(owner, size(), partitions);
		#else
			state[i] = IDLE;
		#endif

		return i + 1;
	}

	// inline void print()
	// {
	// 	fprintf(stderr, "\e[1mmovement\e[0m\n\tidle:");
	// 	for(size_t i = 0; i < partitions[0]; ++i)
	// 		fprintf(stderr, " %zu[%llu %llu (%.2f %.2f)]", i, owner[i], target[i], direction[i].x, direction[i].y);
	// 	fprintf(stderr, "\n\tseeking food:");
	// 	for(size_t i = partitions[0]; i < partitions[1]; ++i)
	// 		fprintf(stderr, " %zu[%llu %llu (%.2f %.2f)]", i, owner[i], target[i], direction[i].x, direction[i].y);
	// 	fprintf(stderr, "\n\tseeking mate:");
	// 	for(size_t i = partitions[1]; i < len; ++i)
	// 		fprintf(stderr, " %zu[%llu %llu (%.2f %.2f)]", i, owner[i], target[i], direction[i].x, direction[i].y);
	// 	fprintf(stderr, "\n");
	// }

	uint64_t * target;
	float * speed;
	glm::vec2 * direction;

	#ifdef DO_PARTITION_ARRAYS
		// |-----0-----|-----1-----|-----2-----|
		// |---idle----|---food----|---mate----|
		// ^0          ^p[0]       ^p[1]       ^size
		size_t partitions[2];
	#else
		enum state_type { IDLE, SEEK_FOOD, SEEK_MATE };
		state_type * state;
	#endif
};

#endif // MOVEMENT_COMPONENT_HPP
