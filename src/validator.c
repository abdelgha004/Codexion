/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:22 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 06:40:53 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	config_validator(t_config *conf)
{
	if (conf->number_of_coders < 1)
		return (6);
	if (conf->time_to_burnout < 1)
		return (7);
	if (conf->time_to_compile < 1)
		return (8);
	if (conf->time_to_debug < 1)
		return (9);
	if (conf->time_to_refactor < 1)
		return (10);
	if (conf->number_of_compiles_required < 1)
		return (11);
	if (conf->dongle_cooldown < 0)
		return (12);
	return (0);
}

int	full_checker(int argc, char **argv, t_config *conf)
{
	int	parser_valid;
	int	config_valid;

	parser_valid = parser(argc, argv, conf);
	if (parser_valid)
		return (parser_valid);
	config_valid = config_validator(conf);
	if (config_valid)
		return (config_valid);
	return (0);
}
