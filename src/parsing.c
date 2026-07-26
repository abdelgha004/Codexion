/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 09:48:41 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/26 10:14:26 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	is_valid_number(char *str)
{
	if (!str || !str[0])
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	check_numbers(char **argv)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!is_valid_number(argv[i]))
			return (0);
		i++;
	}
	return (1);
}

void	fill_simulation(t_simulation *sim, char **argv)
{
	sim->number_of_coders = atoi(argv[1]);
	sim->time_to_burnout = atoi(argv[2]);
	sim->time_to_compile = atoi(argv[3]);
	sim->time_to_debug = atoi(argv[4]);
	sim->time_to_refactor = atoi(argv[5]);
	sim->number_of_compiles_required = atoi(argv[6]);
	sim->dongle_cooldown = atoi(argv[7]);
}

int	check_values(t_simulation *sim)
{
	if (sim->number_of_coders <= 0)
		return (0);
	if (sim->time_to_burnout < 0)
		return (0);
	if (sim->time_to_compile < 0)
		return (0);
	if (sim->time_to_debug < 0)
		return (0);
	if (sim->time_to_refactor < 0)
		return (0);
	if (sim->number_of_compiles_required < 0)
		return (0);
	if (sim->dongle_cooldown < 0)
		return (0);
	return (1);
}

int	parse_args(t_simulation *sim, int argc, char **argv)
{
	if (argc != 9)
		return (1);
	if (!check_numbers(argv))
		return (1);
	fill_simulation(sim, argv);
	if (!check_values(sim))
		return (1);
	if (strcmp(argv[8], "fifo") == 0)
		sim->scheduler = 1;
	else if (strcmp(argv[8], "edf") == 0)
		sim->scheduler = 2;
	else
		return (1);
	return (0);
}
