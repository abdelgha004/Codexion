/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:12:22 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/31 19:19:58 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../codexion.h"

int	create_threads(t_simulation *sim)
{
    int i ;

    i = 0;
    while(i < sim->number_of_coders)
    {
        if(pthread_create(&sim->coders[i].thread,
            NULL, coder_routine, &sim->coders[i]) != 0)
            return (1);
        i++;
    }
    if(pthread_create(&sim->monitor_thread,
        NULL, monitor_routine, sim) != 0)
        {
            printf("Failed to create coder %d\n", i + 1);

            return (1);
        }
    printf("Created coder %d\n", i + 1);
    return (0);
}


int join_threads(t_simulation *sim)
{
    int i ;

    i = 0;

    if(pthread_join(sim->monitor_thread, NULL) != 0)
        return (1);

    while(i < sim->number_of_coders)
    {
        if(pthread_join(sim->coders[i].thread, NULL) != 0)
            return (1);
        i++;
    }
    return (0);
}