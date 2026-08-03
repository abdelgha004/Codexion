/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:12:18 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/31 19:27:44 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"




int take_dongles(t_coder *coder)
{
    /*need to now way we go with the small id first*/
    pthread_mutex_t *first;
    pthread_mutex_t *secend;
    if (coder->left->id < coder->right->id)
    {
        first = &coder->left->mutex;
        secend = &coder->right->mutex;
    }
    else
    {
        first = &coder->right->mutex;
        secend = &coder->left->mutex;
    }
    // pthread_mutex_lock(first);
    // // need to check her 
    // pthread_mutex_lock(secend);
    // // need to check her 
    printf("Coder %d trying first dongle\n", coder->id);
    pthread_mutex_lock(first);
    printf("Coder %d got first dongle\n", coder->id);

    printf("Coder %d trying second dongle\n", coder->id);
    pthread_mutex_lock(secend);
    printf("Coder %d got second dongle\n", coder->id);
    return (0);
}


void	release_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
}

void	compile(t_coder *coder)
{
    long	time;

	// CHANGE: store compile start time safely using mutex protection
	time = get_timestamp(coder->sim);
	set_last_compile_time(coder, time);
    
    pthread_mutex_lock(&coder->sim->print_mutex);
    // should check the result of this
	printf("%ld %d is compiling\n", coder->last_compile_start, coder->id);
	pthread_mutex_unlock(&coder->sim->print_mutex);
    // should check the result of this
    usleep(1000);

    // smart_sleep(coder->sim->time_to_compile, coder->sim);
    coder->compile_count++;
}

void	debug(t_coder *coder)
{
    pthread_mutex_lock(&coder->sim->print_mutex);
    // should check the result of this
	printf("%ld %d is debugging\n",
        get_timestamp(coder->sim),
        coder->id);

	pthread_mutex_unlock(&coder->sim->print_mutex);
    // should check the result of this
    usleep(1000);

    // smart_sleep(coder->sim->time_to_debug, coder->sim);
}

void	refactor(t_coder *coder)
{
    pthread_mutex_lock(&coder->sim->print_mutex);
    // should check the result of this
	printf("%ld %d is refactoring\n",
        get_timestamp(coder->sim),
        coder->id);

	pthread_mutex_unlock(&coder->sim->print_mutex);
    // should check the result of this
    usleep(1000);
    // smart_sleep(coder->sim->time_to_refactor, coder->sim);
}

void *coder_routine(void *arg)
{
    
    t_coder *coder;

    coder = (t_coder *) arg;
    pthread_mutex_lock(&coder->sim->print_mutex);
    printf("Coder %d thread started\n", coder->id);
    pthread_mutex_unlock(&coder->sim->print_mutex);
    while(!get_stop(coder->sim))
    {
        printf("Coder %d: before take_dongles\n", coder->id);

        if (take_dongles(coder) != 0)
            return (NULL);
        printf("Coder %d: after take_dongles\n", coder->id);
        compile(coder);
        release_dongles(coder);
        debug(coder);
        refactor(coder);

    }
    return (NULL);
}
