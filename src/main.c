/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 10:01:27 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/26 12:06:26 by aakourya         ###   ########.fr       */
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
	printf("Coder 1 left dongle: %d\n", sim.coders[0].left->id);
	printf("Coder 1 right dongle: %d\n", sim.coders[0].right->id);
	return (0);
}
