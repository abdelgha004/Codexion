/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 10:01:11 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/31 17:08:49 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int destroy_mutexes(t_simulation *sim)
{
    int i;
    i = 0;

    while(i < sim->number_of_coders)
    {
        if(pthread_mutex_destroy(&sim->dongles[i].mutex) != 0)
            return (1);
        i++;
    }
    if(pthread_mutex_destroy(&sim->print_mutex) != 0)
        return (1);
    if(pthread_mutex_destroy(&sim->data_mutex) != 0)
        return (1);
    return (0);
}