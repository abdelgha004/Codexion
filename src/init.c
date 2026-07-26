/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 10:38:48 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/26 12:04:31 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	init_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].last_release_time = 0;
		// pthread_mutex_init(&sim->dongles[i].mutex, NULL);
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
		sim->coders[i].last_compile_start = 0;
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
	sim->stop = 0;
	return (0);
}
