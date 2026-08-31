/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 10:38:48 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/30 12:23:15 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/* DAY 7: Initialize the mutex and condition variable of each dongle. */
int	init_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].last_release_time = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&sim->dongles[i].condition, NULL) != 0)
        {
            pthread_mutex_destroy(&sim->dongles[i].mutex);
            return (1);
        }
		i++;
	}
	return (0);
}

int	init_coders(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = get_timestamp(sim);
		sim->coders[i].sim = sim;
		i++;
	}
	return (0);
}

void	connect_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].right = &sim->dongles[i];
		if (i == 0)
			sim->coders[i].left = &sim->dongles[sim->number_of_coders - 1];
		else
			sim->coders[i].left = &sim->dongles[i - 1];
		i++;
	}
}

int	init_simulation(t_simulation *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
		return (1);
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!sim->dongles)
	{
		free(sim->coders);
		return (1);
	}
	if (init_coders(sim) != 0)
	{
		free(sim->dongles);
		free(sim->coders);
		return (1);
	}
	if (init_dongles(sim) != 0)
	{
		free(sim->dongles);
		free(sim->coders);
		return (1);
	}
	connect_dongles(sim);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_mutex);
		return (1);
	}
	sim->start_time = get_time_ms();
	sim->stop = 0;
	return (0);
}
