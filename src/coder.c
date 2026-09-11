/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:18 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/11 19:38:32 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int	i;

	conf->start_time = get_time_ms();
	i = 0;
	while (i < conf->number_of_coders)
	{
		pthread_mutex_lock(&conf->coders[i].count_mutex);
		conf->coders[i].last_compile_time = conf->start_time;
		pthread_mutex_unlock(&conf->coders[i].count_mutex);
		i++;
	}
	valid = pthread_create(&conf->monitor_thread, NULL, monitor_routine, conf);
	if (valid)
		return (7);
	count = create_coder_threads(conf);
	join_coder_threads(conf, count);
	if (count < conf->number_of_coders)
		return (7);
	return (0);
}
