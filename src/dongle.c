/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:08 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 14:41:44 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

// static int	wait_for_dongle(t_coder *coder, t_dongle *dongle)
// {
// 	while (!is_sim_end(coder->conf))
// 	{
// 		while ((!dongle->is_available || dongle->heap.size == 0)
// 			&& !is_sim_end(coder->conf))
// 			pthread_cond_wait(&dongle->waiters, &dongle->available_mutex);
// 		if (is_sim_end(coder->conf))
// 			return (1);
// 		if (dongle->heap.nodes[0].id != coder->id)
// 		{
// 			pthread_cond_wait(&dongle->waiters, &dongle->available_mutex);
// 			continue ;
// 		}
// 		if (wait_cooldown(coder, dongle))
// 			continue ;
// 		return (0);
// 	}
// 	return (1);
// }

// static void	print_compile_start(t_coder *coder)
// {
// 	t_config	*conf;

// 	conf = coder->conf;
// 	pthread_mutex_lock(&conf->print_mutex);
// 	if (!is_sim_end(conf))
// 	{
// 		printf("%ld %d has taken a dongle\n",
// 			get_time_ms() - conf->start_time,
// 			coder->id);
// 		// printf("%ld %d has taken a dongle\n",
// 		// 	get_time_ms() - conf->start_time,
// 		// 	coder->id);
// 		printf("%ld %d is compiling\n",
// 			get_time_ms() - conf->start_time,
// 			coder->id);
// 	}
// 	pthread_mutex_unlock(&conf->print_mutex);
// }
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

// int	taking_dongle(t_coder *coder, t_dongle *dongle)
// {
// 	pthread_mutex_lock(&dongle->available_mutex);
// 	if (wait_for_dongle(coder, dongle))
// 	{
// 		pthread_mutex_unlock(&dongle->available_mutex);
// 		return (1);
// 	}
// 	heap_pop(&dongle->heap, coder->conf->scheduler);
// 	dongle->is_available = 0;
// 	pthread_mutex_unlock(&dongle->available_mutex);
// 	return (0);
// }

// int	dongle_logic(t_coder *coder)
// {
// 	int	first;
// 	int	second;

// 	if (coder->conf->number_of_coders == 1)
// 		return (smart_sleep(coder->conf->time_to_burnout, coder->conf), 1);
// 	prepare_and_push_requests(coder);
// 	first = coder->left_dongle;
// 	second = coder->right_dongle;
// 	if (coder->id % 2 == 0)
// 	{
// 		first = coder->right_dongle;
// 		second = coder->left_dongle;
// 	}
// 	if (taking_dongle(coder, &coder->conf->dongles[first])){
// 		cancel_request(coder);//new
// 		return (1);
// 	}
// 	if (taking_dongle(coder, &coder->conf->dongles[second]))
// 	{
// 		release_dongle(&coder->conf->dongles[first]);
// 		cancel_request(coder);//new
// 		return (1);
// 	}
// 	print_compile_start(coder);
// 	return (0);
// }


// int	dongle_logic(t_coder *coder)
// {
// 	int	first;
// 	int	second;

// 	if (coder->conf->number_of_coders == 1)
// 		return (smart_sleep(coder->conf->time_to_burnout, coder->conf), 1);
// 	prepare_and_push_requests(coder);
// 	first = coder->left_dongle;
// 	second = coder->right_dongle;
// 	if (first > second)
// 	{
// 		first = coder->right_dongle;
// 		second = coder->left_dongle;
// 	}
// 	if (taking_dongle(coder, &coder->conf->dongles[first]))
// 	{
// 		cancel_request(&coder->conf->dongles[first], coder->id,
// 			coder->conf->scheduler);
// 		cancel_request(&coder->conf->dongles[second], coder->id,
// 			coder->conf->scheduler);
// 		return (1);
// 	}
// 	if (taking_dongle(coder, &coder->conf->dongles[second]))
// 	{
// 		release_dongle(&coder->conf->dongles[first]);
// 		cancel_request(&coder->conf->dongles[second], coder->id,
// 			coder->conf->scheduler);
// 		return (1);
// 	}
// 	print_compile_start(coder);
// 	return (0);
// }
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
			heap_pop(&dongle_first->heap, coder->conf->scheduler);
			heap_pop(&dongle_second->heap, coder->conf->scheduler);
			dongle_first->is_available = 0;
			dongle_second->is_available = 0;
			pthread_mutex_lock(&coder->conf->print_mutex);
			if (!is_sim_end(coder->conf))
			{
				printf("%ld %d has taken a dongle\n",
					get_time_ms() - coder->conf->start_time, coder->id);
				printf("%ld %d has taken a dongle\n",
					get_time_ms() - coder->conf->start_time, coder->id);
			}
			pthread_mutex_unlock(&coder->conf->print_mutex);
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