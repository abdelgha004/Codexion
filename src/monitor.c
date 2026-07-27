/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:38:26 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/27 09:41:12 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void *monitor_routine(void *arg)
{
    t_simulation *sim;
    sim = (t_simulation *) arg;

    (void)sim; // to remove

    printf("Monitor started\n");
    return (NULL);
}

