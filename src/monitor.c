/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:38:26 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/31 19:14:08 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int check_burnout(t_simulation *sim)
{

    int i;
    long last_compile;
    long current_time;
    
    current_time = get_timestamp(sim);
    i = 0;
    while(i < sim->number_of_coders)
    {
    //         pthread_mutex_lock(&coder->sim->print_mutex);
    // printf("Coder %d thread started\n", coder->id);
    // pthread_mutex_unlock(&coder->sim->print_mutex);
        // printf("Coder %d: current=%ld last=%ld diff=%ld\n",
        //     sim->coders[i].id,
        //     current_time,
        //     last_compile,
        //     current_time - last_compile);
        last_compile = get_last_compile_time(&sim->coders[i]);;
        if(current_time - last_compile >= sim->time_to_burnout)
            return (i);
        i++;
    }
    return (-1);
}



void *monitor_routine(void *arg)
{
    t_simulation *sim;
    int dead_coder;
    
    sim = (t_simulation *) arg;

    pthread_mutex_lock(&sim->print_mutex);
    printf("Monitor thread started\n");
    pthread_mutex_unlock(&sim->print_mutex);
    while (!get_stop(sim))
    {
        dead_coder = check_burnout(sim);
        if (dead_coder != -1)
        {
            set_stop(sim);

            pthread_mutex_lock(&sim->print_mutex);
			printf("%ld %d burned out\n",
				get_timestamp(sim),
				sim->coders[dead_coder].id);
			pthread_mutex_unlock(&sim->print_mutex);
            
            return (NULL);
        }
        usleep(1000);
        
    }
    return (NULL);
}

