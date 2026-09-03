/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:00:00 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/01 10:00:00 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_config	t_config;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;

typedef struct s_request
{
	int		id;
	long	key;
}	t_request;

typedef struct s_heap
{
	t_request	*nodes;
	int			size;
	int			capacity;
}	t_heap;

struct s_dongle
{
	int				id;
	bool			held;
	long			release_time;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_heap			heap;
};

struct s_coder
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	int				compile_count;
	long			last_compile_start;
	int				left;
	int				right;
	t_config		*conf;
};

struct s_config
{
	int				num_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				num_compiles;
	long			dongle_cooldown;
	t_scheduler		scheduler;
	long			start_time;
	bool			running;
	pthread_mutex_t	sim_mutex;
	pthread_mutex_t	print_mutex;
	t_coder			*coders;
	t_dongle		*dongles;
	int				init_coders;
	int				init_dongles;
};

/* parser.c */
int				parse_args(int argc, char **argv, t_config *conf);

/* init.c */
int				init_config(t_config *conf);
int				init_coders(t_config *conf);
int				init_dongles(t_config *conf);

/* cleanup.c */
void			cleanup(t_config *conf);

/* simulation.c */
int				run_simulation(t_config *conf);

/* monitor.c */
void			*monitor_routine(void *arg);

/* coder.c */
void			*coder_routine(void *arg);

/* dongle.c */
int				grab_dongle(t_coder *coder, int id);
void			release_dongle(t_config *conf, int id);

/* heap.c */
void			heap_init(t_heap *heap, int capacity);
int				heap_push(t_heap *heap, t_request req, t_scheduler scheduler);
int				heap_pop(t_heap *heap);
int				heap_peek(t_heap *heap);
void			heap_destroy(t_heap *heap);

/* time.c */
long			get_time_ms(void);
long			elapsed_time(t_config *conf);

/* utils.c */
bool			is_running(t_config *conf);
void			stop_simulation(t_config *conf);
void			wake_all(t_config *conf);
void			print_state(t_coder *coder, char *state);
void			ft_sleep(long ms, t_config *conf);

#endif