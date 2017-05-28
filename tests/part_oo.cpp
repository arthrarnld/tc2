#include "part_oo.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <fstream>
#include <map>
#include <vector>
#include <cmath>

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

    std::vector<particle_emitter *> emitters{EMITTER_COUNT};
    time_point start;
    double taken;

    for(int i = 0; i < EMITTER_COUNT; ++i)
    {
        switch(i % 3)
        {
            case 0:
                emitters[i] = new particle_emitter(glm::vec2(100.0f * i, 100.0f * i), 1);
                break;
            case 1:
                emitters[i] = new cone_emitter(glm::vec2(100.0f * i, 100.0f * i), 1, M_PI/6.0f);
                break;
            case 2:
                emitters[i] = new area_emitter(glm::vec2(100.0f * i, 100.0f * i), 1, 5);
                break;
        }
    }

    int particle_count;
    for(int i = 0; i < ITERATIONS; ++i)
    {
        particle_count = 0;

        start = now();
        for(size_t j = 0; j < EMITTER_COUNT; ++j)
        {
            particle_emitter * e = emitters[j];
            e->tick(1);

            particle_count += e->get_particle_count();
        }
        taken = elapsed(start, now());

        times[particle_count] += taken;
        ++occurrences[particle_count];

        printf("\ri: %d\tdt: %-20f", i, taken);

        // log("particle count: %d\t time taken: %f", e.get_particle_count(), taken);
        if(i % INCREASE == 0)
            for(size_t j = 0; j < EMITTER_COUNT; ++j)
                emitters[j]->set_emission_rate(emitters[j]->get_emission_rate() + 1);
    }

    printf("\n");

    std::ofstream file("out_part_oo");
    for(auto & p : times)
    {
        file << p.first << '\t' << p.second / occurrences[p.first] << '\n';
    }
}
