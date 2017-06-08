#include "ecs_do.hpp"
#include "common/log.hpp"
#include "common/time.hpp"
#include "common/util.hpp"

#include <iostream>

#include <stdio.h>
#include <unistd.h>

// Variables for measuring time
time_point tp;

struct run {
	int index;
	size_t entities;
	double accum_time;
	size_t remaining_passes;
};

void populate(world & w, size_t count)
{
	for(size_t i = 0; i < count; ++i)
	{
		uint64_t e = w.create();
		int s = irand(0, 7);
		w.spe.create(e, s);
		w.pos.create(e, drand(0, 100), drand(0, 100));

		if(s != 1 && s != 4)
		{
			w.hea.create(e, drand(2,8), 1 + irand(0,1)*3);
			w.mov.create(e, drand(10,18));
			w.rep.create(e, drand(1,4));
		}
	}
}

void run_measure_time(size_t entities, size_t increment, size_t passes, size_t iterations)
{
	fprintf(stderr, "# Time test\n# Entities: %zu\n# Increment: %zu\n# Passes: %zu\n# Iterations: %zu\n",
		entities, increment, passes, iterations);

	size_t run_count = entities/increment;
	size_t total_passes = run_count * passes;
	size_t passes_completed = 0;

	double * times = new double[run_count];

	std::vector<run> runs { run_count, { 0, 0, 0.0, passes } };
	size_t i = 0;
	for(size_t i = 0; i < run_count; ++i) {
		runs[i].index = i;
		runs[i].entities = (i+1) * increment;
	}

	while(!runs.empty())
	{
		// fprintf(stderr, "\e[1;32m-----\e[0;30;42m PASS %zu \e[1;32;49m-----\e[0m\n", passes_completed);
		int idx = random() % runs.size();
		run & this_run = runs[idx];

		world w;
		populate(w, this_run.entities);
		A(w.count() == this_run.entities);

		fprintf(stderr, "\r%6.2f%%", 100.0f*(float)passes_completed/(float)total_passes);

		for(size_t i = 0; i < iterations; ++i)
		{
			// fprintf(stderr, "\n\e[1;34mIteration %zu\e[0m\n", i);
			// w.print();
			tp = now();
			w.update(1);
			this_run.accum_time += elapsed(tp, now());
		}

		if(--this_run.remaining_passes == 0) {
			times[this_run.index] = this_run.accum_time / (double)(iterations*passes);
			runs.erase(runs.begin()+idx);
		}

		++passes_completed;
	}
	fprintf(stderr, "\r   100%%\n");

	for(size_t i = 0; i < run_count; ++i)
		printf("%llu\t%f\n", (i+1)*increment, times[i]);

	delete [] times;
}

// void run_measure_insertion()
// {
// 	fprintf(stderr, "# Insertion test\n# Entity count: %llu\n", entity_count);
//
// 	world w;
//
// 	double * times = new double[entity_count+1];
// 	for(size_t i = 0; i < entity_count; ++i)
// 	{
// 		int s = i%6;
// 		tp = now();
//
// 		uint64_t e = w.create();
// 		w.spe.create(e, s);
// 		w.hea.create(e, appetites[s], (s+1)%6);
// 		w.mov.create(e, speeds[s]);
// 		w.pos.create(e, sin(i)*10, cos(i)*10);
// 		w.rep.create(e, libidos[s]);
//
// 		times[w.count()] = elapsed(tp, now());
//
// 		fprintf(stderr, "\r%3d%%", 100*(i+1)/entity_count);
// 	}
// 	fprintf(stderr, "\n");
//
// 	for(int i = 0; i <= entity_count; ++i)
// 		std::cout << i << '\t' << times[i] << '\n';
// 	delete [] times;
// }
//
// void run_measure_fps()
// {
// 	fprintf(stderr, "# FPS test\n");
//
// 	world w;
// 	double accum_time;
// 	size_t count = 0;
//
// 	while(true)
// 	{
// 		// Create new entity
// 		uint64_t e = w.create();
// 		int s = count%6;
// 		w.spe.create(e, s);
// 		w.hea.create(e, appetites[s], (s+1)%6);
// 		w.mov.create(e, speeds[s]);
// 		w.pos.create(e, sin(count)*10, cos(count)*10);
// 		w.rep.create(e, libidos[s]);
// 		++count;
// 		// Reset time count
// 		accum_time = 0.0;
//
// 		for(size_t i = 0; i < 10; ++i)
// 		{
// 			tp = now();
// 			w.update(1);
// 			accum_time += elapsed(tp, now());
// 		}
// 		double avg_time = accum_time / 10.0;
// 		fprintf(stderr, "\r%-6.2f%%", 6000.0f*avg_time);
//
// 		if(avg_time > 1.0/60.0) {
// 			fprintf(stderr, "\r100%%   \n");
// 			printf("%zu\n", count-1);
// 			return;
// 		}
// 	}
// }

// void test_debug(size_t iterations)
// {
// 	world w;
//
// 	{
// 		uint64_t e = w.create();
// 		w.spe.create(e, 0);
// 		w.pos.create(e, -10, 0);
// 		w.hea.create(e, 1, 1);
// 		w.rep.create(e, );
// 		w.mov.create(e, );
// 	}
// }

int main(int argc, char ** argv)
{
	size_t seed;
	FILE * fp = fopen("/dev/urandom", "rb");
	fread(&seed, sizeof(seed), 1, fp);
	srandom(seed);
	fclose(fp);

	enum { TICK, INSERTION, FPS } test = TICK;

	int c;
	while((c = getopt(argc, argv, "tif")) != -1)
		switch(c)
		{
			case 't':
				test = TICK;
				break;
			case 'i':
				test = INSERTION;
				break;
			case 'f':
				test = FPS;
				break;
			case '?':
				fprintf(stderr, "unknown option '-%c'\n", optopt);
			default:
				return -1;
		}

	size_t entities;
	size_t increment;
	size_t iterations;
	size_t passes;

	switch(test)
	{
		case TICK:
			if(optind != argc-4)
				fatal("tick test requires four arguments: <entities> <increment> <passes> <iterations>");
			entities = atoll(argv[optind++]);
			increment = atoll(argv[optind++]);
			passes = atoll(argv[optind++]);
			iterations = atoll(argv[optind++]);
			run_measure_time(entities, increment, passes, iterations);
			break;
		case INSERTION:
			// if(optind != argc-1)
			// 	fatal("insertion test requires one argument: entity count");
			// entity_count = atoll(argv[optind]);
			break;
		case FPS:
			break;
			// if(optind != argc-1)
			// 	fatal("FPS test requires one argument: increment period");
			// increment_period = atoll(argv[optind]);
	}

	return 0;
}
