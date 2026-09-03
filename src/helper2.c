#include "../codexion.h"

int	get_stop(t_simulation *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->stop_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stop);
}

void	set_stop(t_simulation *sim)
{
	int	i;

	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_cond_broadcast(&sim->dongles[i].condition);
		i++;
	}
}