#include "../codexion.h"

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				i;
	long			now;
	long			deadline;

	sim = (t_simulation *)arg;
	while (!get_stop(sim))
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			now = get_timestamp(sim);
			deadline = sim->coders[i].last_compile_start
				+ sim->time_to_burnout;
			if (now >= deadline)
			{
				pthread_mutex_lock(&sim->print_mutex);
				printf("%ld %d burned out\n",
					now, sim->coders[i].id);
				pthread_mutex_unlock(&sim->print_mutex);
				set_stop(sim);
				return (NULL);
			}
			i++;
		}
		usleep(500);
	}
	return (NULL);
}