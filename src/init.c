/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:53:56 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 13:22:30 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	init_global_mutexes(t_config *conf)
{
	if (pthread_mutex_init(&conf->print_mutex, NULL))
		return (21);
	conf->is_print_init = 1;
	if (pthread_mutex_init(&conf->end_mutex, NULL))
		return (21);
	conf->is_end_init = 1;
	return (0);
}

static int	allocate_memory(t_config *conf)
{
	int	n;

	n = conf->number_of_coders;
	conf->coders = malloc(sizeof(t_coder) * n);
	conf->dongles = malloc(sizeof(t_dongle) * n);
	if (!conf->coders || !conf->dongles)
		return (16);
	return (0);
}

static int	init_coders(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->number_of_coders)
	{
		conf->coders[i].id = i + 1;
		conf->coders[i].left_dongle = i;
		conf->coders[i].right_dongle = (i + 1) % conf->number_of_coders;
		conf->coders[i].compile_count = 0;
		conf->coders[i].last_compile_time = get_time_ms();
		conf->coders[i].conf = conf;
		if (pthread_mutex_init(&conf->coders[i].count_mutex, NULL))
			return (i);
		i++;
	}
	return (i);
}

static int	init_dongles(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->number_of_coders)
	{
		conf->dongles[i].id = i;
		conf->dongles[i].is_available = 1;
		conf->dongles[i].last_release_time = 0;
		heap_init(&conf->dongles[i].heap, conf->number_of_coders);
		if (!conf->dongles[i].heap.nodes)
			return (i);
		if (pthread_mutex_init(&conf->dongles[i].available_mutex, NULL))
			return (i);
		if (pthread_cond_init(&conf->dongles[i].waiters, NULL))
			return (i);
		i++;
	}
	return (i);
}

int	initialize_data(t_config *conf)
{
	int	error;

	error = init_global_mutexes(conf);
	if (error)
		return (error);
	error = allocate_memory(conf);
	if (error)
		return (error);
	conf->initialized_coders = init_coders(conf);
	if (conf->initialized_coders != conf->number_of_coders)
		return (22);
	conf->initialized_dongles = init_dongles(conf);
	if (conf->initialized_dongles != conf->number_of_coders)
		return (22);
	return (0);
}

// int	allocate_memory(t_config *conf)
// {
// 	conf->coders = malloc(sizeof(t_coder) * conf->number_of_coders);
// 	conf->dongles = malloc(sizeof(t_dongle) * conf->number_of_coders);
// 	if (!conf->coders || !conf->dongles)
// 		return (16);
// 	return (0);
// }

// int	init_coders(t_config *conf)
// {
// 	int		i;
// 	int		mtx;

// 	i = 0;
// 	while (i < conf->number_of_coders)
// 	{
// 		conf->coders[i].id = i + 1;
// 		conf->coders[i].left_dongle = i;
// 		conf->coders[i].right_dongle = (i + 1) % conf->number_of_coders;
// 		conf->coders[i].compile_count = 0;
// 		conf->coders[i].last_compile_time = get_time_ms();
// 		conf->coders[i].conf = conf;
// 		if (pthread_mutex_init(&conf->coders[i].count_mutex, NULL))
// 			return (i);
// 		i++;
// 	}
// 	return (i);
// }

// static int	init_dongles(t_config *conf)
// {
// 	t_dongle	tmp;
// 	int			i;
// 	int			mtx;

// 	i = 0;
// 	while (i < conf->number_of_coders)
// 	{
// 		tmp.id = i;
// 		tmp.is_available = 1;
// 		tmp.last_release_time = 0;
// 		heap_init(&tmp.heap, conf->number_of_coders);
// 		if (!tmp.heap.nodes)
// 			return (i);
// 		conf->dongles[i] = tmp;
// 		mtx = pthread_mutex_init(&conf->dongles[i].available_mutex, NULL);
// 		if (mtx)
// 			return (i);
// 		mtx = pthread_cond_init(&conf->dongles[i].waiters, NULL);
// 		if (mtx)
// 			return (pthread_mutex_destroy(
// 					&conf->dongles[i].available_mutex), i);
// 		i++;
// 	}
// 	return (conf->number_of_coders);
// }

// int	initialize_data(t_config *conf)
// {
// 	int	valid;

// 	valid = pthread_mutex_init(&conf->print_mutex, NULL);
// 	if (valid)
// 		return (21);
// 	conf->is_print_init = 1;
// 	valid = pthread_mutex_init(&conf->end_mutex, NULL);
// 	if (valid)
// 		return (21);
// 	conf->is_end_init = 1;
// 	valid = allocate_memory(conf);
// 	if (valid)
// 		return (valid);
// 	conf->initialized_coders = init_coders(conf);
// 	if (conf->initialized_coders < conf->number_of_coders)
// 		return (22);
// 	conf->initialized_dongles = init_dongles(conf);
// 	if (conf->initialized_dongles < conf->number_of_coders)
// 		return (22);
// 	return (0);
// }
