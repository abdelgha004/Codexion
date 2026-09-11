/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:53:49 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/11 19:59:00 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	print_error(int error)
{
	if (error == 1)
		fprintf(stderr, "Error: invalid number of arguments.\n");
	else if (error == 2)
		fprintf(stderr, "Error: empty argument.\n");
	else if (error == 3)
		fprintf(stderr, "Error: integer overflow.\n");
	else if (error == 4)
		fprintf(stderr, "Error: invalid argument.\n");
	else if (error == 5)
		fprintf(stderr, "Error: invalid scheduler.\n");
	else if (error == 6)
		fprintf(stderr, "Error: invalid configuration.\n");
	else if (error == 7)
		fprintf(stderr, "Error: thread creation failed.\n");
	else if (error == 8)
		fprintf(stderr, "Error: memory allocation failed.\n");
	else if (error == 10)
		fprintf(stderr, "Error: initialization failed.\n");
}

static void	setup_arguments(int **targets, t_config *conf)
{
	targets[0] = &conf->number_of_coders;
	targets[1] = &conf->time_to_burnout;
	targets[2] = &conf->time_to_compile;
	targets[3] = &conf->time_to_debug;
	targets[4] = &conf->time_to_refactor;
	targets[5] = &conf->number_of_compiles_required;
	targets[6] = &conf->dongle_cooldown;
}

int	parse_arguments(int argc, char **argv, t_config *conf)
{
	int	i;
	int	*targets[7];
	int	error;

	i = 0;
	if (argc != 9)
		return (1);
	setup_arguments(targets, conf);
	while (++i < 8)
	{
		error = parse_int(argv[i], targets[i - 1]);
		if (error)
			return (error);
	}
	if (!strcmp(argv[i], "fifo"))
		conf->scheduler = 0;
	else if (!strcmp(argv[i], "edf"))
		conf->scheduler = 1;
	else
		return (5);
	return (0);
}
