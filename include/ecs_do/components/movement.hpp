#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "../soa_utils.hpp"
#include <functional>
#include <iostream>

using swap_func = std::function<void(size_t,size_t)>;
#define MEMBER_SWAP_FUNC [this](size_t a, size_t b){this->swap(a,b);}

template<size_t N>
size_t move(size_t index, int dst, swap_func && swap, size_t (&partitions) [N])
{
	int p = N;
	while(p > 0 && index < partitions[p-1])
		--p;

	while(p < dst) // moving right
	{
		swap(index, partitions[p] - 1);
		index = partitions[p] - 1;
		--partitions[p];
		++p;
	}
	while(p > dst) // moving left
	{
		swap(index, partitions[p-1]);
		index = partitions[p-1];
		++partitions[p-1];
		--p;
	}
	return index;
}

template<typename T, size_t N>
void print_partitioned(const T * array, size_t size, const size_t (&partitions)[N])
{
	using namespace std;
	int p = 0;
	cout << "[ ";
	size_t i = 0;
	for(int p = 0; p <= N; ++p)
	{
		if(p > 0)
			cout << "| ";
		size_t max = p == N ? size : partitions[p];
		for(; i < max; ++i)
			cout << array[i] << " ";
	}
	cout << "]" << endl;
}

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

	SOA_COMPONENT_BASE(movement)

	inline size_t create(uint64_t e, float s) {
		size_t i = create(e);
		speed[i] = s;

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
		#ifdef DO_PARTITION_ARRAYS
			move(i, 0, MEMBER_SWAP_FUNC, partitions);
			// print_partitioned(owner, size(), partitions);
		#else
			state[i] = IDLE;
		#endif

		return i + 1;
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
