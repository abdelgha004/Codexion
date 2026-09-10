/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:20 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 11:01:14 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	smart_sleep(long time_to_sleep, t_config *conf)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < time_to_sleep)
	{
		if (is_sim_end(conf))
			return ;
		usleep(500);
	}
}

void	print_state(t_coder *coder, char *state)
{
	t_config	*conf;

	conf = coder->conf;
	pthread_mutex_lock(&conf->print_mutex);
	if (is_sim_end(conf))
	{
		pthread_mutex_unlock(&conf->print_mutex);
		return ;
	}
	printf("%ld %d %s\n", get_timestamp(conf), coder->id, state);
	pthread_mutex_unlock(&conf->print_mutex);
}

void	broadcast_waiters(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->initialized_dongles)
	{
		pthread_mutex_lock(&conf->dongles[i].available_mutex);
		pthread_cond_broadcast(&conf->dongles[i].waiters);
		pthread_mutex_unlock(&conf->dongles[i].available_mutex);
		i++;
	}
}
