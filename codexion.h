
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
	int					id;
	long				enter_time;
	long				time_to_burnout;
}						t_request;

typedef struct s_heap
{
	t_request			*nodes;
	int					size;
	int					capacity;
}						t_heap;

typedef struct s_dongle
{
	int					id;
	long				time_of_last_released;
	int					is_available;
	t_heap				heap;
	pthread_mutex_t		available_mutex;
	pthread_cond_t		waiters;
}						t_dongle;

typedef struct s_coder
{
	int					id;
	long				last_compile_time;
	int					compile_count;
	int					right_dongle;
	int					left_dongle;
	t_config			*conf;
	pthread_mutex_t		count_mutex;
	pthread_t			thread;

}						t_coder;

typedef struct s_config
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	int					scheduler;
	long				start_time;
	pthread_mutex_t		end_mutex;
	pthread_mutex_t		print_mutex;
	int					is_print_init;
	int					is_end_init;
	int					simulation_ends;
	int					initialized_coders;
	int					initialized_dongles;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_t			monitor_thread;
}						t_config;

int						parser(int argc, char **argv, t_config *conf);
int						config_validator(t_config *conf);
int						initialize_data(t_config *conf);
int						full_checker(int argc, char **argv, t_config *conf);
int						ft_atoi(const char *str, int *result);
void					ft_perror(int i);
void					*coder(void *args);
void					coder_compile(t_coder *coder);
int						is_coder_finished(t_coder *coder);
void					coder_debug_refactor(t_coder *coder);
int						create_coder_threads(t_config *conf);
void					join_coder_threads(t_config *conf, int count);
int						run_simulation(t_config *conf);
void					clean_data(t_config *conf);
long					current_time(void);
void					print_state(t_coder *coder, char *state);
int						taking_dongle(t_coder *coder, t_dongle *dongle);
int						dongle_logic(t_coder *coder);
void					check_ifended(t_config *conf);
int						is_sim_end(t_config *conf);
void					release_dongle(t_dongle *dongle);
void					push_request(t_coder *coder, t_dongle *dongle,
							t_request *req);
void					prepare_and_push_requests(t_coder *coder);
void					handle_cooldown(t_coder *coder, t_dongle *dongle);
void					ft_usleep(long time_to_sleep, t_config *conf);
void					ft_broadcast(t_config *conf);
int						is_higher_priority(t_request a, t_request b,
							int scheduler);
void					heapify_up(t_heap *heap, int index, int scheduler);
void					heapify_down(t_heap *heap, int index, int scheduler);
void					*monitor_routine(void *args);

void					heap_init(t_heap *heap, int capacity);
void					heap_free(t_heap *heap);
int						heap_push(t_heap *heap, t_request *node, int scheduler);
t_request				heap_pop(t_heap *heap, int scheduler);
#endif
