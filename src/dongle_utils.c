/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 07:02:39 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/11 20:07:50 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->available_mutex);
	dongle->last_release_time = get_time_ms();
	dongle->is_available = 1;
	pthread_cond_broadcast(&dongle->waiters);
	pthread_mutex_unlock(&dongle->available_mutex);
}

int	wait_cooldown(t_coder *coder, t_dongle *dongle)
{
	long			wait;
	struct timeval	tv;
	struct timespec	ts;

	wait = coder->conf->dongle_cooldown - (get_time_ms()
			- dongle->last_release_time);
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
	add_request(&dongle->heap, req, coder->conf->scheduler);
	pthread_cond_broadcast(&dongle->waiters);
	pthread_mutex_unlock(&dongle->available_mutex);
}

void	cancel_request(t_dongle *dongle, int coder_id, int scheduler)
{
	pthread_mutex_lock(&dongle->available_mutex);
	remove_request(&dongle->heap, coder_id, scheduler);
	pthread_cond_broadcast(&dongle->waiters);
	pthread_mutex_unlock(&dongle->available_mutex);
}

void	prepare_and_push_requests(t_coder *coder)
{
	t_config	*conf;
	t_request	req;

	conf = coder->conf;
	req.id = coder->id;
	req.enter_time = get_time_ms();
	pthread_mutex_lock(&coder->count_mutex);
	req.time_to_burnout = coder->last_compile_time + conf->time_to_burnout;
	pthread_mutex_unlock(&coder->count_mutex);
	push_request(coder, &conf->dongles[coder->left_dongle], &req);
	push_request(coder, &conf->dongles[coder->right_dongle], &req);
}
