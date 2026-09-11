/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:08 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/11 20:06:43 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	print_compile_start(t_coder *coder)
{
	t_config	*conf;

	conf = coder->conf;
	pthread_mutex_lock(&conf->print_mutex);
	if (!is_sim_end(conf))
		printf("%ld %d is compiling\n",
			get_time_ms() - conf->start_time,
			coder->id);
	pthread_mutex_unlock(&conf->print_mutex);
}

int	dongle_logic(t_coder *coder)
{
	int	first;
	int	second;

	if (coder->conf->number_of_coders == 1)
		return (smart_sleep(coder->conf->time_to_burnout, coder->conf), 1);
	prepare_and_push_requests(coder);
	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first > second)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (taking_dongles(coder, first, second))
	{
		cancel_request(&coder->conf->dongles[first], coder->id,
			coder->conf->scheduler);
		cancel_request(&coder->conf->dongles[second], coder->id,
			coder->conf->scheduler);
		return (1);
	}
	print_compile_start(coder);
	return (0);
}

static int	dongles_ready(t_coder *coder, t_dongle *first,
		t_dongle *second)
{
	long	now;

	now = get_time_ms();
	if (!first->is_available || !second->is_available)
		return (0);
	if (first->heap.size == 0 || second->heap.size == 0)
		return (0);
	if (first->heap.nodes[0].id != coder->id
		|| second->heap.nodes[0].id != coder->id)
		return (0);
	if (now - first->last_release_time < coder->conf->dongle_cooldown)
		return (0);
	if (now - second->last_release_time < coder->conf->dongle_cooldown)
		return (0);
	return (1);
}

void	take_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	remove_top_request(&first->heap, coder->conf->scheduler);
	remove_top_request(&second->heap, coder->conf->scheduler);
	first->is_available = 0;
	second->is_available = 0;
	pthread_mutex_lock(&coder->conf->print_mutex);
	if (!is_sim_end(coder->conf))
	{
		printf("%ld %d has taken a dongle\n",
			get_time_ms() - coder->conf->start_time, coder->id);
		printf("%ld %d has taken a dongle\n",
			get_time_ms() - coder->conf->start_time, coder->id);
	}
	pthread_mutex_unlock(&coder->conf->print_mutex);
}

int	taking_dongles(t_coder *coder, int first, int second)
{
	t_dongle	*dongle_first;
	t_dongle	*dongle_second;

	dongle_first = &coder->conf->dongles[first];
	dongle_second = &coder->conf->dongles[second];
	while (!is_sim_end(coder->conf))
	{
		pthread_mutex_lock(&dongle_first->available_mutex);
		pthread_mutex_lock(&dongle_second->available_mutex);
		if (dongles_ready(coder, dongle_first, dongle_second))
		{
			take_dongles(coder, dongle_first, dongle_second);
			pthread_mutex_unlock(&dongle_second->available_mutex);
			pthread_mutex_unlock(&dongle_first->available_mutex);
			return (0);
		}
		pthread_mutex_unlock(&dongle_second->available_mutex);
		pthread_mutex_unlock(&dongle_first->available_mutex);
		usleep(100);
	}
	return (1);
}
