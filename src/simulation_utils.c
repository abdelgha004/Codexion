/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:53:45 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 06:41:12 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	coder_compile(t_coder *coder)
{
	t_config	*conf;

	conf = coder->conf;
	pthread_mutex_lock(&coder->count_mutex);
	coder->last_compile_time = current_time();
	pthread_mutex_unlock(&coder->count_mutex);
	ft_usleep(conf->time_to_compile, conf);
	release_dongle(&conf->dongles[coder->right_dongle]);
	release_dongle(&conf->dongles[coder->left_dongle]);
	pthread_mutex_lock(&coder->count_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->count_mutex);
}

int	is_coder_finished(t_coder *coder)
{
	int	finished;

	finished = 0;
	if (is_sim_end(coder->conf))
	{
		pthread_mutex_lock(&coder->conf->end_mutex);
		coder->conf->simulation_ends = 1;
		pthread_mutex_unlock(&coder->conf->end_mutex);
	}
	pthread_mutex_lock(&coder->count_mutex);
	if (coder->compile_count == coder->conf->number_of_compiles_required)
		finished = 1;
	pthread_mutex_unlock(&coder->count_mutex);
	return (finished);
}

void	coder_debug_refactor(t_coder *coder)
{
	t_config	*conf;

	conf = coder->conf;
	print_state(coder, "is debugging");
	ft_usleep(conf->time_to_debug, conf);
	print_state(coder, "is refactoring");
	ft_usleep(conf->time_to_refactor, conf);
}

int	create_coder_threads(t_config *conf)
{
	int	i;
	int	valid;

	i = 0;
	while (i < conf->number_of_coders)
	{
		valid = pthread_create(&conf->coders[i].thread, NULL, coder,
				&conf->coders[i]);
		if (valid)
		{
			pthread_mutex_lock(&conf->end_mutex);
			conf->simulation_ends = 1;
			pthread_mutex_unlock(&conf->end_mutex);
			ft_broadcast(conf);
			return (i);
		}
		i++;
	}
	return (i);
}

void	join_coder_threads(t_config *conf, int count)
{
	int	j;

	j = 0;
	while (j < count)
		pthread_join(conf->coders[j++].thread, NULL);
	pthread_join(conf->monitor_thread, NULL);
}
