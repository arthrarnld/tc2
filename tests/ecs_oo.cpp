#include "ecs_oo.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

// #include <unistd.h>
// #include <stdlib.h>
#include <iostream>

float appetites [] = {  20,  10,  10,  10,  10,  10 };
float preys     [] = {  10,  20,  10,  10,  10,  10 };
float libidos   [] = {   1,   1, 0.2,   1,   1,   1 };
float speeds    [] = {   1,   1,   1,   1,   1,   2 };

world w;


int main(int argc, char ** argv)
{
	if(argc != 3)
	{
		printf("usage: %s entity-count increment-period\n", argv[0]);
		return -1;
	}

	size_t entity_count = atoll(argv[1]);
	size_t increment_period = atoll(argv[2]);

	double times[entity_count+1];

	w.push_system(new health_system);
	w.push_system(new reproduction_system);
	w.push_system(new movement_system);

	size_t printed_size = 0;
	time_point tp;
	double time;
	for(size_t i = 0; i < entity_count * increment_period; ++i)
	{
		if(i % increment_period == 0)
		{
			auto & e = w.create();
			int s = i%6;
			e.add_component<species_component>(s);
			e.add_component<health_component>(appetites[s], (s+1)%6);
			e.add_component<movement_component>(speeds[s]);
			e.add_component<position_component>(sin(i)*100, 0);
			e.add_component<reproduction_component>(libidos[s]);
			times[w.get_entity_count()] = 0.0;
		}
		tp = now();
		w.update(1);
		time = elapsed(tp, now());
		times[w.get_entity_count()] += time;

		if(printed_size != w.get_entity_count()) {
			fprintf(stderr, "\r%-10llu %-10f", w.get_entity_count(), time);
			printed_size = w.get_entity_count();
		}
	}
	fprintf(stderr, "\n");

	for(int i = 0; i <= entity_count; ++i)
		std::cout << i << '\t' << times[i] / (double)increment_period << '\n';

	return 0;
}
