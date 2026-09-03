#include "../codexion.h"

static int	create_coders(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_coders)
	{
		conf->coders[i].last_compile_start = conf->start_time;
		if (pthread_create(&conf->coders[i].thread, NULL,
				coder_routine, &conf->coders[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	join_coders(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_coders)
	{
		pthread_join(conf->coders[i].thread, NULL);
		i++;
	}
}

int	run_simulation(t_config *conf)
{
	pthread_t	monitor;

	conf->start_time = get_time_ms();
	conf->running = true;
	if (create_coders(conf))
	{
		stop_simulation(conf);
		wake_all(conf);
		return (1);
	}
	if (pthread_create(&monitor, NULL, monitor_routine, conf))
	{
		stop_simulation(conf);
		wake_all(conf);
		join_coders(conf);
		return (1);
	}
	join_coders(conf);
	pthread_join(monitor, NULL);
	return (0);
}
