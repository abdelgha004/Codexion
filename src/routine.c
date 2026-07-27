/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:12:18 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/27 09:12:19 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"


void *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *) arg;

    printf("coder %d started\n", coder->id);
    return (NULL);
}