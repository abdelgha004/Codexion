/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

bool	is_running(t_config *conf)
{
	bool	value;

	pthread_mutex_lock(&conf->sim_mutex);
	value = conf->running;
	pthread_mutex_unlock(&conf->sim_mutex);
	return (value);
}

void	stop_simulation(t_config *conf)
{
	pthread_mutex_lock(&conf->sim_mutex);
	conf->running = false;
	pthread_mutex_unlock(&conf->sim_mutex);
}

void	wake_all(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_coders)
	{
		pthread_mutex_lock(&conf->dongles[i].mutex);
		pthread_cond_broadcast(&conf->dongles[i].cond);
		pthread_mutex_unlock(&conf->dongles[i].mutex);
		i++;
	}
}

void	print_state(t_coder *coder, char *state)
{
	t_config	*conf;

	conf = coder->conf;
	pthread_mutex_lock(&conf->print_mutex);
	if (is_running(conf))
		printf("%ld %d %s\n", elapsed_time(conf), coder->id, state);
	pthread_mutex_unlock(&conf->print_mutex);
}

void	ft_sleep(long ms, t_config *conf)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms && is_running(conf))
		usleep(500);
}