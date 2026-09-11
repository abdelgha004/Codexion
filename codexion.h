/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 06:42:46 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/11 20:20:43 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_config	t_config;

typedef struct s_request
{
	int		id;
	long	enter_time;
	long	time_to_burnout;
}	t_request;

typedef struct s_heap
{
	t_request	*nodes;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	long			last_release_time;
	int				is_available;
	t_heap			heap;
	pthread_mutex_t	available_mutex;
	pthread_cond_t	waiters;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	long			last_compile_time;
	int				compile_count;
	int				right_dongle;
	int				left_dongle;
	t_config		*conf;
	pthread_mutex_t	count_mutex;
	pthread_t		thread;
}	t_coder;

typedef struct s_config
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				scheduler;
	long			start_time;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	print_mutex;
	int				is_print_init;
	int				is_end_init;
	int				simulation_ends;
	int				initialized_coders;
	int				initialized_dongles;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor_thread;
}	t_config;

/* Parsing */
int			parse_arguments(int argc, char **argv, t_config *conf);
int			config_validator(t_config *conf);
int			validate_arguments(int argc, char **argv, t_config *conf);
int			parse_int(const char *str, int *result);
void		print_error(int i);

/* Initialization */
int			initialize_simulation(t_config *conf);

/* Coder simulation */
int			run_simulation(t_config *conf);
void		*coder(void *args);
void		coder_compile(t_coder *coder);
void		coder_debug_refactor(t_coder *coder);
int			is_coder_finished(t_coder *coder);

/* Threads */
int			create_coder_threads(t_config *conf);
void		join_coder_threads(t_config *conf, int count);

/* Dongle management */
int			taking_dongles(t_coder *coder, int first, int second);
int			dongle_logic(t_coder *coder);
void		release_dongle(t_dongle *dongle);
int			wait_cooldown(t_coder *coder, t_dongle *dongle);
void		push_request(t_coder *coder, t_dongle *dongle, t_request *req);
void		cancel_request(t_dongle *dongle, int coder_id, int scheduler);
void		prepare_and_push_requests(t_coder *coder);
void		take_dongles(t_coder *coder, t_dongle *first, t_dongle *second);

/* Scheduler */
int			is_higher_priority(t_request first_req,
				t_request second_req, int scheduler);
void		move_node_up(t_heap *heap, int index, int scheduler);
void		move_node_down(t_heap *heap, int index, int scheduler);
void		add_request(t_heap *heap, t_request *node, int scheduler);
t_request	remove_top_request(t_heap *heap, int scheduler);
void		heap_init(t_heap *heap, int capacity);
void		remove_request(t_heap *heap, int id, int scheduler);
void		remove_heap_node(t_heap *heap, int i, int scheduler);

/* Monitor */
void		*monitor_routine(void *args);
void		check_ifended(t_config *conf);
void		check_burnout(t_config *conf);
int			is_sim_end(t_config *conf);

/* Time and printing */
long		get_time_ms(void);
void		smart_sleep(long time_to_sleep, t_config *conf);
void		print_state(t_coder *coder, char *state);
void		broadcast_waiters(t_config *conf);
long		get_timestamp(t_config *conf);

/* Cleanup */
void		cleanup_simulation(t_config *conf);

#endif