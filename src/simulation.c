
#include "../codexion.h"

void	*coder(void *args)
{
	t_config	*conf;
	t_coder		*coder;

	coder = (t_coder *)(args);
	conf = coder->conf;
	if (coder->id % 2)
		usleep(500);
	while (!is_sim_end(conf))
	{
		if (dongle_logic(coder))
			return (NULL);
		coder_compile(coder);
		if (is_coder_finished(coder))
			return (NULL);
		coder_debug_refactor(coder);
	}
	return (NULL);
}

int	run_simulation(t_config *conf)
{
	int	count;
	int	valid;

	conf->start_time = current_time();
	valid = pthread_create(&conf->monitor_thread, NULL, monitor_routine, conf);
	if (valid)
		return (14);
	count = create_coder_threads(conf);
	join_coder_threads(conf, count);
	if (count < conf->number_of_coders)
		return (14);
	return (0);
}
