/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:08 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 06:38:37 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	while (!is_sim_end(coder->conf))
	{
		while ((!dongle->is_available || dongle->heap.size == 0)
			&& !is_sim_end(coder->conf))
			pthread_cond_wait(&dongle->waiters, &dongle->available_mutex);
		if (is_sim_end(coder->conf))
			return (1);
		if (dongle->heap.nodes[0].id != coder->id)
		{
			pthread_cond_wait(&dongle->waiters, &dongle->available_mutex);
			continue ;
		}
		if (wait_cooldown(coder, dongle))
			continue ;
		return (0);
	}
	return (1);
}

static void	print_compile_start(t_coder *coder)
{
	t_config	*conf;

	conf = coder->conf;
	pthread_mutex_lock(&conf->print_mutex);
	if (!is_sim_end(conf))
	{
		printf("%ld %d has taken a dongle\n",
			current_time() - conf->start_time,
			coder->id);
		printf("%ld %d has taken a dongle\n",
			current_time() - conf->start_time,
			coder->id);
		printf("%ld %d is compiling\n",
			current_time() - conf->start_time,
			coder->id);
	}
	pthread_mutex_unlock(&conf->print_mutex);
}

int	taking_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->available_mutex);
	if (wait_for_dongle(coder, dongle))
	{
		pthread_mutex_unlock(&dongle->available_mutex);
		return (1);
	}
	heap_pop(&dongle->heap, coder->conf->scheduler);
	dongle->is_available = 0;
	pthread_mutex_unlock(&dongle->available_mutex);
	return (0);
}

int	dongle_logic(t_coder *coder)
{
	int	first;
	int	second;

	if (coder->conf->number_of_coders == 1)
		return (ft_usleep(coder->conf->time_to_burnout, coder->conf), 1);
	prepare_and_push_requests(coder);
	first = coder->left_dongle;
	second = coder->right_dongle;
	if (coder->id % 2 == 0)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (taking_dongle(coder, &coder->conf->dongles[first]))
		return (1);
	if (taking_dongle(coder, &coder->conf->dongles[second]))
	{
		release_dongle(&coder->conf->dongles[first]);
		return (1);
	}
	print_compile_start(coder);
	return (0);
}
