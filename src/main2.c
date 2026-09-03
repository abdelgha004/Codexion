/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 10:01:27 by aakourya          #+#    #+#             */
/*   Updated: 2026/08/31 17:24:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (parse_args(&sim, argc, argv) != 0)
		return (1);
	if (init_simulation(&sim) != 0)
		return (1);
	if (create_threads(&sim) != 0)
		return (1);
	if (join_threads(&sim) != 0)
		return (1);
	destroy_mutexes(&sim);
	// free(sim.coders);
	// free(sim.dongles);
	return (0);
}
