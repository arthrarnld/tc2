#include "ood/entity.hpp"
#include <vector>

int main()
{
	std::vector<entity> entities;
	for(int i = 0; i < 10; ++i)
		entities.emplace_back();
	return 0;
}
