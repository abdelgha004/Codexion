/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:53:54 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 10:22:55 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(int argc, char **argv)
{
	t_config	conf;
	int			valid;

	valid = 0;
	memset(&conf, 0, sizeof(t_config));
	valid = full_checker(argc, argv, &conf);
	if (valid)
		return (print_error(valid), valid);
	valid = initialize_data(&conf);
	if (!valid)
		valid = run_simulation(&conf);
	return (print_error(valid), clean_data(&conf), valid);
}
