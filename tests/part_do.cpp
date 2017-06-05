#include "part_do.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <unistd.h>
#include <iostream>
#include <map>

using test_func_type = void(*)();
test_func_type test_func;

int ITERATIONS;
int INCREASE;
size_t EMITTER_COUNT;

void run_measure_tick()
{
    std::map<int, double> times;
    std::map<int, int> occurrences;

    particle_system ps;
    time_point start;
    double taken;

    for(int i = 0; i < EMITTER_COUNT; ++i)
        ps.create_emitter(glm::vec2(100.0 * i, 100.0 * i), 1);

    int particle_count;
    for(int i = 0; i < ITERATIONS; ++i)
    {
        particle_count = 0;

        start = now();
        particle_count = ps.tick(1);
        taken = elapsed(start, now());

        times[particle_count] += taken;
        ++occurrences[particle_count];

        fprintf(stderr, "\ri: %d\tdt: %-20f", i, taken);

        // log("particle count: %d\t time taken: %f", e.get_particle_count(), taken);
        if(i % INCREASE == 0)
            for(size_t j = 0; j < ps.m_last_active_emitter; ++j)
                ps.e_emission_rates[j]++;
    }

    fprintf(stderr, "\n");

    for(auto & p : times)
    {
        std::cout << p.first << '\t' << p.second / occurrences[p.first] << '\n';
    }
}

void run_measure_insertion()
{
    particle_system ps;

    std::map<int, double> times;
    time_point start;
    double taken;

    for(int i = 0; i < EMITTER_COUNT; ++i)
    {
        start = now();
        ps.create_emitter(glm::vec2(100.0f * i, 100.0f * i), 1);
        taken = elapsed(start, now());

        times[i] = taken;
        fprintf(stderr, "\ri: %d\tdt: %-20f", i, taken);
    }

    fprintf(stderr, "\n");

    for(auto & p : times)
    {
        std::cout << p.first << '\t' << p.second << '\n';
    }
}

int main(int argc, char ** argv)
{
    enum { TICK, INSERTION } test;
	test_func = run_measure_tick;
	test = TICK;
	int c;
	while((c = getopt(argc, argv, "ti")) != -1)
		switch(c)
		{
			case 't':
				test_func = run_measure_tick;
				test = TICK;
				break;
			case 'i':
				test_func = run_measure_insertion;
				test = INSERTION;
				break;
			case '?':
				fprintf(stderr, "unknown option '-%c'\n", optopt);
			default:
				return -1;
		}

	switch(test)
	{
		case TICK:
			if(optind != argc-3)
				fatal("tick test requires three arguments: iteration count, increment period and emitter count");
            ITERATIONS = atoll(argv[optind]);
            INCREASE = atoll(argv[optind+1]);
			EMITTER_COUNT = atoll(argv[optind+2]);
			break;
		case INSERTION:
			if(optind != argc-1)
				fatal("insertion test requires one argument: emitter count");
			EMITTER_COUNT = atoll(argv[optind]);
			break;
	}

	test_func();

	return 0;
}
