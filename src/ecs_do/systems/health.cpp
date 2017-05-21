#include "systems/health.hpp"
#include "world.hpp"

bool update_health(world * w, double dt)
{
	health & h = w->hea;
	for(size_t i = 0; i < h.size(); ++i)
	{
		if(h.health_points[i] > 0)
			h.health_points[i] = std::min(
				(double)h.max_health[i],
				h.health_points[i] + h.regen_rate[i] * dt
			);
		else
			w->kill(h.owner[i]);
	}
	return true;
}
