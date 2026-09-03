/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	take_two(t_coder *coder)
{
	int	first;
	int	second;

	first = coder->left;
	second = coder->right;
	if (first > second)
	{
		first = coder->right;
		second = coder->left;
	}
	if (grab_dongle(coder, first))
		return (1);
	if (grab_dongle(coder, second))
	{
		release_dongle(coder->conf, first);
		return (1);
	}
	return (0);
}

static void	compile(t_coder *coder)
{
	t_config	*conf;

	conf = coder->conf;
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	print_state(coder, "is compiling");
	ft_sleep(conf->time_to_compile, conf);
	release_dongle(conf, coder->left);
	release_dongle(conf, coder->right);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
}

static int	finished(t_coder *coder)
{
	int	count;

	pthread_mutex_lock(&coder->mutex);
	count = coder->compile_count;
	pthread_mutex_unlock(&coder->mutex);
	return (count >= coder->conf->num_compiles);
}

static void	debug_refactor(t_coder *coder)
{
	t_config	*conf;

	conf = coder->conf;
	print_state(coder, "is debugging");
	ft_sleep(conf->time_to_debug, conf);
	print_state(coder, "is refactoring");
	ft_sleep(conf->time_to_refactor, conf);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = arg;
	if (coder->conf->num_coders == 1)
	{
		grab_dongle(coder, coder->left);
		while (is_running(coder->conf))
			ft_sleep(1, coder->conf);
		release_dongle(coder->conf, coder->left);
		return (NULL);
	}
	while (is_running(coder->conf))
	{
		if (take_two(coder))
			break ;
		compile(coder);
		if (finished(coder))
			break ;
		debug_refactor(coder);
	}
	return (NULL);
}