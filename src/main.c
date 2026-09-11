/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:53:54 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/11 19:25:51 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(int argc, char **argv)
{
	t_config	config;
	int			status;

	status = 0;
	memset(&config, 0, sizeof(t_config));
	status = validate_arguments(argc, argv, &config);
	if (status)
		return (print_error(status), status);
	status = initialize_simulation(&config);
	if (!status)
		status = run_simulation(&config);
	print_error(status);
	cleanup_simulation(&config);
	return (status);
}
