/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	cleanup_coders(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->init_coders)
		pthread_mutex_destroy(&conf->coders[i++].mutex);
	free(conf->coders);
	conf->coders = NULL;
}

static void	cleanup_dongles(t_config *conf)
{
	int	i;

	i = 0;
	while (i < conf->init_dongles)
	{
		heap_destroy(&conf->dongles[i].heap);
		pthread_mutex_destroy(&conf->dongles[i].mutex);
		pthread_cond_destroy(&conf->dongles[i].cond);
		i++;
	}
	free(conf->dongles);
	conf->dongles = NULL;
}

static void	cleanup_mutexes(t_config *conf)
{
	pthread_mutex_destroy(&conf->print_mutex);
	pthread_mutex_destroy(&conf->sim_mutex);
}

void	cleanup(t_config *conf)
{
	if (conf->coders)
		cleanup_coders(conf);
	if (conf->dongles)
		cleanup_dongles(conf);
	cleanup_mutexes(conf);
}