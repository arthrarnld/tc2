#include "systems/health.hpp"
#include "common/log.hpp"
#include "world.hpp"

#include "components/movement.hpp"

bool update_health(world * w, double dt)
{
	health & h = w->hea;
	movement & m = w->mov;
	#ifdef DO_PARTITION_ARRAYS
		// Idle
		for(size_t i = 0; i < h.partitions[0]; ++i)
		{
			h.hunger[i] += h.appetite[i] * dt;
		}
		// Eating
		for(size_t i = h.partitions[0]; i < h.size(); ++i)
		{
			--h.hunger[i];
			if(h.hunger[i] <= 0) // done eating
			{
				h.hunger[i] = 0;
				h.stop_eating(i);
				m.stop_seeking_food(m.lookup(h.owner[i]));
				debug("%llu satisfied", h.owner[i]);
			}
		}
	#else
		for(size_t i = 0; i < h.size(); ++i)
		{
			if(h.state[i] == health::EATING)
			{
				--h.hunger[i];
				if(h.hunger[i] <= 0)
				{
					h.hunger[i] = 0;
					h.stop_eating(i);
					m.stop_seeking_food(m.lookup(h.owner[i]));
					debug("%llu satisfied", h.owner[i]);
				}
			}
			else
				h.hunger[i] += h.appetite[i] * dt;
		}
	#endif

	return true;
}
