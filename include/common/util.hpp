#ifndef UTIL_HPP
#define UTIL_HPP

#include <functional>
#include <iostream>
#include <cstdlib>

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

inline float norm_rand()
{
    return (float)random() / RAND_MAX;
}

#endif // UTIL_HPP
