#include "ecs_do.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <iostream>

float appetites [] = {  20,  10,  10,  10,  10,  10 };
float preys     [] = {  10,  20,  10,  10,  10,  10 };
float libidos   [] = {   1,   1, 0.2,   1,   1,   1 };
float speeds    [] = {  20,  15,  20,  20,  15,  25 };

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
			uint64_t e = w.create();
			int s = i%6;
			w.spe.create(e, s);
			w.hea.create(e, appetites[s], (s+1)%6);
			w.mov.create(e, speeds[s]);
			w.pos.create(e, sin(i)*10, cos(i)*10);
			w.rep.create(e, libidos[s]);
			times[w.count()] = 0.0;
		}
		tp = now();
		w.update(1);
		times[w.count()] += elapsed(tp, now());

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

		uint64_t e = w.create();
		w.spe.create(e, s);
		w.hea.create(e, appetites[s], (s+1)%6);
		w.mov.create(e, speeds[s]);
		w.pos.create(e, sin(i)*10, cos(i)*10);
		w.rep.create(e, libidos[s]);

		times[w.count()] = elapsed(tp, now());

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

	test_func();

	for(int i = 0; i <= entity_count; ++i)
		std::cout << i << '\t' << times[i] / (double)increment_period << '\n';

	delete [] times;
	return 0;
}
