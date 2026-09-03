/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static long	request_key(t_coder *coder)
{
	if (coder->conf->scheduler == FIFO)
		return (get_time_ms());
	return (coder->last_compile_start
		+ coder->conf->time_to_burnout);
}

static void	add_request(t_coder *coder, t_dongle *dongle)
{
	t_request	req;

	req.id = coder->id;
	req.key = request_key(coder);
	heap_push(&dongle->heap, req, coder->conf->scheduler);
}

static int	cooldown_done(t_coder *coder, t_dongle *dongle)
{
	long	wait_until;

	wait_until = dongle->release_time + coder->conf->dongle_cooldown;
	return (get_time_ms() >= wait_until);
}

int	grab_dongle(t_coder *coder, int id)
{
	t_dongle	*dongle;

	dongle = &coder->conf->dongles[id];
	pthread_mutex_lock(&dongle->mutex);
	add_request(coder, dongle);
	while (is_running(coder->conf))
	{
		if (!dongle->held && heap_peek(&dongle->heap) == coder->id
			&& cooldown_done(coder, dongle))
			break ;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (!is_running(coder->conf))
		return (pthread_mutex_unlock(&dongle->mutex), 1);
	heap_pop(&dongle->heap);
	dongle->held = true;
	pthread_mutex_unlock(&dongle->mutex);
	print_state(coder, "has taken a dongle");
	return (0);
}

void	release_dongle(t_config *conf, int id)
{
	t_dongle	*dongle;

	dongle = &conf->dongles[id];
	pthread_mutex_lock(&dongle->mutex);
	dongle->held = false;
	dongle->release_time = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}