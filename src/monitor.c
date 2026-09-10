/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:53:51 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 06:40:40 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	is_sim_end(t_config *conf)
{
	int	verdict;

	pthread_mutex_lock(&conf->end_mutex);
	verdict = conf->simulation_ends;
	pthread_mutex_unlock(&conf->end_mutex);
	return (verdict);
}

void	check_ifended(t_config *conf)
{
	int	i;
	int	ended;

	ended = 1;
	i = 0;
	while (i < conf->number_of_coders)
	{
		pthread_mutex_lock(&conf->coders[i].count_mutex);
		if (conf->coders[i].compile_count < conf->number_of_compiles_required)
			ended = 0;
		pthread_mutex_unlock(&conf->coders[i].count_mutex);
		i++;
	}
	if (ended)
	{
		pthread_mutex_lock(&conf->end_mutex);
		conf->simulation_ends = 1;
		pthread_mutex_unlock(&conf->end_mutex);
	}
}

void	check_burnout(t_config *conf)
{
	int		i;
	long	last_time;
	int		count;

	i = 0;
	while (i < conf->number_of_coders)
	{
		pthread_mutex_lock(&conf->coders[i].count_mutex);
		last_time = conf->coders[i].last_compile_time;
		count = conf->coders[i].compile_count;
		pthread_mutex_unlock(&conf->coders[i].count_mutex);
		if (count < conf->number_of_compiles_required && last_time
			+ conf->time_to_burnout <= current_time())
		{
			pthread_mutex_lock(&conf->print_mutex);
			pthread_mutex_lock(&conf->end_mutex);
			conf->simulation_ends = 1;
			pthread_mutex_unlock(&conf->end_mutex);
			printf("%ld %d burned out\n", current_time() - conf->start_time,
				conf->coders[i].id);
			pthread_mutex_unlock(&conf->print_mutex);
			break ;
		}
		i++;
	}
}

void	*monitor_routine(void *args)
{
	t_config	*conf;

	conf = (t_config *)args;
	while (1)
	{
		usleep(500);
		check_ifended(conf);
		check_burnout(conf);
		if (is_sim_end(conf))
		{
			ft_broadcast(conf);
			break ;
		}
	}
	return (NULL);
}
