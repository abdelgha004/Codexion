/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 10:01:32 by aakourya          #+#    #+#             */
/*   Updated: 2026/07/27 09:25:15 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_simulation	t_simulation;
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;

typedef struct s_simulation
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;

	t_dongle	*dongles;
	t_coder		*coders;

	pthread_t monitor_thread;
	
	int						stop;
	// pthread_mutex_t print_mutex;
	int						scheduler;
}							t_simulation;

typedef struct s_dongle
{
	int						id;
	long					last_release_time;
	// pthread_mutex_t mutex;
}							t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	int				compile_count;
	long			last_compile_start;
	t_simulation	*sim;
}							t_coder;

int		parse_args(t_simulation *sim, int argc, char **argv);
int		init_simulation(t_simulation *sim);
void	*coder_routine(void *arg);
int	create_threads(t_simulation *sim);
int join_threads(t_simulation *sim);
void *monitor_routine(void *arg);

#endif
