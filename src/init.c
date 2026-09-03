/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	init_config(t_config *conf)
{
	conf->running = true;
	if (pthread_mutex_init(&conf->sim_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&conf->print_mutex, NULL))
		return (pthread_mutex_destroy(&conf->sim_mutex), 1);
	conf->coders = calloc(conf->num_coders, sizeof(t_coder));
	conf->dongles = calloc(conf->num_coders, sizeof(t_dongle));
	if (!conf->coders || !conf->dongles)
		return (1);
	return (0);
}

int	init_coders(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_coders)
	{
		conf->coders[i].id = i + 1;
		conf->coders[i].left = i;
		conf->coders[i].right = (i + 1) % conf->num_coders;
		conf->coders[i].conf = conf;
		conf->coders[i].last_compile_start = conf->start_time;
		if (pthread_mutex_init(&conf->coders[i].mutex, NULL))
			return (1);
		conf->init_coders++;
		i++;
	}
	return (0);
}

int	init_dongles(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->num_coders)
	{
		conf->dongles[i].id = i;
		conf->dongles[i].held = false;
		conf->dongles[i].release_time = 0;
		heap_init(&conf->dongles[i].heap, conf->num_coders);
		if (pthread_mutex_init(&conf->dongles[i].mutex, NULL))
			return (1);
		if (pthread_cond_init(&conf->dongles[i].cond, NULL))
			return (1);
		conf->init_dongles++;
		i++;
	}
	return (0);
}