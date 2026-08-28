/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:12:18 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/30 15:50:37 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	take_dongles(t_coder *coder)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (coder->left->id < coder->right->id)
	{
		first = &coder->left->mutex;
		second = &coder->right->mutex;
	}
	else
	{
		first = &coder->right->mutex;
		second = &coder->left->mutex;
	}
	pthread_mutex_lock(first);
	if (get_stop(coder->sim))
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	pthread_mutex_lock(&coder->sim->print_mutex);
	printf("%ld %d has taken a dongle\n",
		get_timestamp(coder->sim), coder->id);
	pthread_mutex_unlock(&coder->sim->print_mutex);
	pthread_mutex_lock(second);
	if (get_stop(coder->sim))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return (1);
	}
	pthread_mutex_lock(&coder->sim->print_mutex);
	printf("%ld %d has taken a dongle\n",
		get_timestamp(coder->sim), coder->id);
	pthread_mutex_unlock(&coder->sim->print_mutex);
	return (0);
}


void	release_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
}

void	compile(t_coder *coder)
{
	coder->last_compile_start = get_timestamp(coder->sim);
	pthread_mutex_lock(&coder->sim->print_mutex);
	printf("%ld %d is compiling\n",
		coder->last_compile_start, coder->id);
	pthread_mutex_unlock(&coder->sim->print_mutex);
	smart_sleep(coder->sim->time_to_compile, coder->sim);
	coder->compile_count++;
	if (all_coders_finished(coder->sim))
		set_stop(coder->sim);
}

void	debug(t_coder *coder)
{
    pthread_mutex_lock(&coder->sim->print_mutex);
    // should check the result of this
	printf("%ld %d is debugging\n",
        get_timestamp(coder->sim),
        coder->id);
	pthread_mutex_unlock(&coder->sim->print_mutex);
    // should check the result of this
    smart_sleep(coder->sim->time_to_debug, coder->sim);
}

void	refactor(t_coder *coder)
{
    pthread_mutex_lock(&coder->sim->print_mutex);
    // should check the result of this
	printf("%ld %d is refactoring\n",
        get_timestamp(coder->sim),
        coder->id);

	pthread_mutex_unlock(&coder->sim->print_mutex);
    // should check the result of this

    smart_sleep(coder->sim->time_to_refactor, coder->sim);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!get_stop(coder->sim))
	{
		if (take_dongles(coder) != 0)
			return (NULL);
		compile(coder);
		release_dongles(coder);
		if (get_stop(coder->sim))
			break;
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}

int	all_coders_finished(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (sim->coders[i].compile_count
			< sim->number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}