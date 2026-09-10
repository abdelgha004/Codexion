/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 10:44:25 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 11:05:59 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

long	get_timestamp(t_config *conf)
{
	return (get_time_ms() - conf->start_time);
}

int	parse_int(const char *str, int *result)
{
	long	number;
	int		sign;

	number = 0;
	sign = 1;
	if (*str == '\0')
		return (2);
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0')
		return (4);
	while (*str >= '0' && *str <= '9')
	{
		number = number * 10 + (*str - '0');
		if (number * sign > 2147483647 || number * sign < -2147483648)
			return (3);
		str++;
	}
	if (*str)
		return (4);
	*result = number * sign;
	return (0);
}
