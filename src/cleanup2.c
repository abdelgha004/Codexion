#include "../codexion.h"

int	destroy_mutexes(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_mutex_destroy(&sim->dongles[i].mutex) != 0)
			return (1);
		if (pthread_cond_destroy(&sim->dongles[i].condition) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_destroy(&sim->print_mutex) != 0)
		return (1);
	if (pthread_mutex_destroy(&sim->stop_mutex) != 0)
		return (1);
	return (0);
}