
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	parse_number(char *str, long *value)
{
	long	n;
	int		i;

	n = 0;
	i = 0;
	if (!str[0])
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		if (n > (LONG_MAX - (str[i] - '0')) / 10)
			return (1);
		n = n * 10 + (str[i++] - '0');
	}
	*value = n;
	return (0);
}

static int	set_values(char **av, t_config *conf)
{
	long	v[7];
	int		i;

	i = 0;
	while (i < 7)
	{
		if (parse_number(av[i + 1], &v[i]))
			return (1);
		i++;
	}
	conf->num_coders = v[0];
	conf->time_to_burnout = v[1];
	conf->time_to_compile = v[2];
	conf->time_to_debug = v[3];
	conf->time_to_refactor = v[4];
	conf->num_compiles = v[5];
	conf->dongle_cooldown = v[6];
	return (0);
}

static int	validate_values(t_config *conf)
{
	if (conf->num_coders < 1 || conf->num_coders > 3000)
		return (1);
	if (conf->time_to_burnout < 1)
		return (1);
	if (conf->time_to_compile < 1)
		return (1);
	if (conf->time_to_debug < 1 || conf->time_to_refactor < 1)
		return (1);
	if (conf->num_compiles < 1 || conf->dongle_cooldown < 0)
		return (1);
	return (0);
}

static int	set_scheduler(char *str, t_config *conf)
{
	if (!strcmp(str, "fifo") || !strcmp(str, "FIFO"))
		conf->scheduler = FIFO;
	else if (!strcmp(str, "edf") || !strcmp(str, "EDF"))
		conf->scheduler = EDF;
	else
		return (1);
	return (0);
}

int	parse_args(int argc, char **argv, t_config *conf)
{
	if (argc != 9)
	{
		fprintf(stderr, "Usage: ./codexion <7 args> <fifo|edf>\n");
		return (1);
	}
	if (set_values(argv, conf))
		return (fprintf(stderr, "Error: invalid argument\n"), 1);
	if (validate_values(conf))
		return (fprintf(stderr, "Error: invalid value\n"), 1);
	if (set_scheduler(argv[8], conf))
		return (fprintf(stderr, "Error: scheduler must be fifo or edf\n"), 1);
	return (0);
}