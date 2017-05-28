#include "ecs_oo.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

// #include <unistd.h>
// #include <stdlib.h>
#include <iostream>
#include <cstring>

float appetites [] = {  20,  10,  10,  10,  10,  10 };
float preys     [] = {  10,  20,  10,  10,  10,  10 };
float libidos   [] = {   1,   1, 0.2,   1,   1,   1 };
float speeds    [] = {   1,   1,   1,   1,   1,   2 };

world w;

using test_func_type = void(*)();

// Function that runs the tests
test_func_type test_func;
// Number of entities to reach
size_t entity_count;
// Number of iterations between consecutive
// entity births
size_t increment_period;
// Accumulated times by number of entities
double * times;
// Variables for measuring time
time_point tp;


void run_measure_tick()
{
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
		times[w.get_entity_count()] += elapsed(tp, now());

		fprintf(stderr, "\r%3d%%", 100*(i+1)/(entity_count*increment_period));
	}
	fprintf(stderr, "\n");
}

void run_measure_insertion()
{
	for(size_t i = 0; i < entity_count; ++i)
	{
		int s = i%6;
		tp = now();

		auto & e = w.create();
		e.add_component<species_component>(s);
		e.add_component<health_component>(appetites[s], (s+1)%6);
		e.add_component<movement_component>(speeds[s]);
		e.add_component<position_component>(sin(i)*100, 0);
		e.add_component<reproduction_component>(libidos[s]);

		times[w.get_entity_count()] = elapsed(tp, now());

		fprintf(stderr, "\r%3d%%", 100*(i+1)/entity_count);
	}
	fprintf(stderr, "\n");
}

int main(int argc, char ** argv)
{
	if(argc < 3)
	{
		printf("usage: %s [OPTIONS] entity-count increment-period\n", argv[0]);
		return -1;
	}

	test_func = run_measure_tick;
	int opt = 1;
	// Parse OPTIONS
	for( ; opt < argc-2; ++opt)
	{
		if(!strcmp(argv[opt], "--insertion"))
			test_func = run_measure_insertion;
		else if(strcmp(argv[opt], "--tick"))
			fatal("unknown option '%s'", argv[opt]);
	}

	entity_count = atoll(argv[opt++]);
	increment_period = atoll(argv[opt++]);
	times = new double[entity_count+1];

	fprintf(stderr, "test: %s\nentity count: %llu\nincrement period: %llu\n\n",
		test_func == run_measure_tick ? "tick" : "insertion",
		entity_count,
		increment_period
	);

	w.push_system(new health_system);
	w.push_system(new reproduction_system);
	w.push_system(new movement_system);

	test_func();

	for(int i = 0; i <= entity_count; ++i)
		std::cout << i << '\t' << times[i] / (double)increment_period << '\n';

	delete [] times;
	return 0;
}
