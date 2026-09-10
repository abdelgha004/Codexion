/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:53:56 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 06:39:46 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	allocate_memory(t_config *conf)
{
	int	n;

	n = conf->number_of_coders;
	conf->coders = malloc(n * sizeof(t_coder));
	conf->dongles = malloc(n * sizeof(t_dongle));
	if (!conf->coders || !conf->dongles)
		return (16);
	return (0);
}

static int	init_coders(t_config *conf)
{
	int		i;
	int		n;
	int		mtx;
	t_coder	tmp;

	n = conf->number_of_coders;
	i = 0;
	while (i < n)
	{
		tmp.id = i + 1;
		tmp.left_dongle = i;
		tmp.right_dongle = (i + 1) % n;
		tmp.compile_count = 0;
		tmp.last_compile_time = current_time();
		tmp.conf = conf;
		conf->coders[i] = tmp;
		mtx = pthread_mutex_init(&conf->coders[i].count_mutex, NULL);
		if (mtx)
			return (i);
		i++;
	}
	return (n);
}

static int	init_dongles(t_config *conf)
{
	t_dongle	tmp;
	int			i;
	int			mtx;

	i = 0;
	while (i < conf->number_of_coders)
	{
		tmp.id = i;
		tmp.is_available = 1;
		tmp.time_of_last_released = 0;
		heap_init(&tmp.heap, conf->number_of_coders);
		if (!tmp.heap.nodes)
			return (i);
		conf->dongles[i] = tmp;
		mtx = pthread_mutex_init(&conf->dongles[i].available_mutex, NULL);
		if (mtx)
			return (i);
		mtx = pthread_cond_init(&conf->dongles[i].waiters, NULL);
		if (mtx)
			return (pthread_mutex_destroy(
					&conf->dongles[i].available_mutex), i);
		i++;
	}
	return (conf->number_of_coders);
}

int	initialize_data(t_config *conf)
{
	int	valid;

	valid = pthread_mutex_init(&conf->print_mutex, NULL);
	if (valid)
		return (21);
	conf->is_print_init = 1;
	valid = pthread_mutex_init(&conf->end_mutex, NULL);
	if (valid)
		return (21);
	conf->is_end_init = 1;
	valid = allocate_memory(conf);
	if (valid)
		return (valid);
	conf->initialized_coders = init_coders(conf);
	if (conf->initialized_coders < conf->number_of_coders)
		return (22);
	conf->initialized_dongles = init_dongles(conf);
	if (conf->initialized_dongles < conf->number_of_coders)
		return (22);
	return (0);
}
