#include "part_do.hpp"
#include "common/log.hpp"
#include "common/time.hpp"
#include <fstream>

#include <map>

std::map<int, double> times;
std::map<int, int> occurrences;

int main(int argc, char ** argv)
{
    if(argc != 4)
    {
        error("usage: part_oo <iteration_count> <increase> <emitter_count>");
        return 1;
    }

    const int ITERATIONS = atoi(argv[1]);
    const int INCREASE = atoi(argv[2]);
    const size_t EMITTER_COUNT = atoi(argv[3]);

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

        printf("\ri: %d\tdt: %-20f", i, taken);

        // log("particle count: %d\t time taken: %f", e.get_particle_count(), taken);
        if(i % INCREASE == 0)
            for(size_t j = 0; j < ps.m_last_active_emitter; ++j)
                ps.e_emission_rates[j]++;
    }

    std::ofstream file("out_part_do");
    for(auto & p : times)
    {
        file << p.first << '\t' << p.second / occurrences[p.first] << '\n';
    }
}
