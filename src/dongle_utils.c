/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:03 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 06:36:29 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_t	*mtx;

	mtx = &dongle->available_mutex;
	pthread_mutex_lock(mtx);
	dongle->time_of_last_released = current_time();
	dongle->is_available = 1;
	pthread_cond_broadcast(&dongle->waiters);
	pthread_mutex_unlock(mtx);
}

int	wait_cooldown(t_coder *coder, t_dongle *dongle)
{
	long			wait;
	struct timeval	tv;
	struct timespec	ts;

	wait = coder->conf->dongle_cooldown - (current_time()
			- dongle->time_of_last_released);
	if (wait <= 0)
		return (0);
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (tv.tv_usec + wait * 1000) / 1000000;
	ts.tv_nsec = ((tv.tv_usec + wait * 1000) % 1000000) * 1000;
	pthread_cond_timedwait(&dongle->waiters, &dongle->available_mutex, &ts);
	return (1);
}

void	push_request(t_coder *coder, t_dongle *dongle, t_request *req)
{
	pthread_mutex_lock(&dongle->available_mutex);
	heap_push(&dongle->heap, req, coder->conf->scheduler);
	pthread_cond_broadcast(&dongle->waiters);
	pthread_mutex_unlock(&dongle->available_mutex);
}

void	prepare_and_push_requests(t_coder *coder)
{
	t_config	*conf;
	t_request	req;

	conf = coder->conf;
	req.id = coder->id;
	req.enter_time = current_time();
	pthread_mutex_lock(&coder->count_mutex);
	req.time_to_burnout = coder->last_compile_time + conf->time_to_burnout;
	pthread_mutex_unlock(&coder->count_mutex);
	push_request(coder, &conf->dongles[coder->left_dongle], &req);
	push_request(coder, &conf->dongles[coder->right_dongle], &req);
}
