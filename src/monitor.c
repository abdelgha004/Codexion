/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	check_finished(t_config *conf)
{
	int	i;
	int	count;

	i = 0;
	while (i < conf->num_coders)
	{
		pthread_mutex_lock(&conf->coders[i].mutex);
		count = conf->coders[i].compile_count;
		pthread_mutex_unlock(&conf->coders[i].mutex);
		if (count < conf->num_compiles)
			return (0);
		i++;
	}
	return (1);
}

static int	check_burnout(t_config *conf)
{
	int		i;
	long	last;

	i = 0;
	while (i < conf->num_coders)
	{
		pthread_mutex_lock(&conf->coders[i].mutex);
		last = conf->coders[i].last_compile_start;
		if (conf->coders[i].compile_count < conf->num_compiles
			&& get_time_ms() - last >= conf->time_to_burnout)
		{
			pthread_mutex_unlock(&conf->coders[i].mutex);
			printf("%ld %d burned out\n", elapsed_time(conf),
				conf->coders[i].id);
			stop_simulation(conf);
			return (1);
		}
		pthread_mutex_unlock(&conf->coders[i].mutex);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_config	*conf;

	conf = arg;
	while (is_running(conf))
	{
		if (check_finished(conf) || check_burnout(conf))
		{
			stop_simulation(conf);
			wake_all(conf);
			break ;
		}
		usleep(500);
	}
	return (NULL);
}