#include "../codexion.h"

long	get_last_compile_time(t_coder *coder)
{
	long	time;

	pthread_mutex_lock(&coder->sim->data_mutex);
	time = coder->last_compile_start;
	pthread_mutex_unlock(&coder->sim->data_mutex);

	return (time);
}

void	set_last_compile_time(t_coder *coder, long time)
{
	pthread_mutex_lock(&coder->sim->data_mutex);
	coder->last_compile_start = time;
	pthread_mutex_unlock(&coder->sim->data_mutex);
}

int	get_stop(t_simulation *sim)
{    
	int	stop;

	pthread_mutex_lock(&sim->data_mutex);

	stop = sim->stop;

	pthread_mutex_unlock(&sim->data_mutex);

	return (stop);
}


void	set_stop(t_simulation *sim)
{
	pthread_mutex_lock(&sim->data_mutex);

	sim->stop = 1;

	pthread_mutex_unlock(&sim->data_mutex);
}