/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:54:05 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 06:35:33 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	clean_data(t_config *conf)
{
	int	i;

	i = 0;
	if (conf->coders)
	{
		while (i < conf->initialized_coders)
			pthread_mutex_destroy(&conf->coders[i++].count_mutex);
		free(conf->coders);
	}
	i = 0;
	if (conf->dongles)
	{
		while (i < conf->initialized_dongles)
		{
			pthread_mutex_destroy(&conf->dongles[i].available_mutex);
			pthread_cond_destroy(&conf->dongles[i].waiters);
			free(conf->dongles[i].heap.nodes);
			i++;
		}
		free(conf->dongles);
	}
	if (conf->is_end_init)
		pthread_mutex_destroy(&conf->end_mutex);
	if (conf->is_print_init)
		pthread_mutex_destroy(&conf->print_mutex);
}
