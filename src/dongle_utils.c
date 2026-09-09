
#include "../codexion.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_t	*mtx;

	mtx = &dongle->available_mutex;
	pthread_mutex_lock(mtx);
	dongle->time_of_last_released = current_time();
	dongle->is_available = 1;
	pthread_cond_broadcast(&dongle->waiters);
	pthread_mutex_unlock(mtx);
}

void	push_request(t_coder *coder, t_dongle *dongle, t_request *req)
{
	pthread_mutex_lock(&dongle->available_mutex);
	if (heap_push(&dongle->heap, req, coder->conf->scheduler))
	{
		pthread_mutex_unlock(&dongle->available_mutex);
		return ;
	}
	pthread_cond_broadcast(&dongle->waiters);
	pthread_mutex_unlock(&dongle->available_mutex);
}


void	handle_cooldown(t_coder *coder, t_dongle *dongle)
{
	struct timeval	now;
	struct timespec	wait_until;
	long			remaining;

	if (dongle->time_of_last_released <= 0)
		return ;
	remaining = coder->conf->dongle_cooldown
		- (current_time() - dongle->time_of_last_released);
	if (remaining <= 0)
		return ;
	gettimeofday(&now, NULL);
	wait_until.tv_sec = now.tv_sec + remaining / 1000;
	wait_until.tv_nsec = (now.tv_usec * 1000)
		+ (remaining % 1000) * 1000000;
	if (wait_until.tv_nsec >= 1000000000)
	{
		wait_until.tv_sec++;
		wait_until.tv_nsec -= 1000000000;
	}
	while (remaining > 0 && !is_sim_end(coder->conf))
	{
		if (pthread_cond_timedwait(&dongle->waiters,
				&dongle->available_mutex, &wait_until) != 0)
			break ;
		remaining = coder->conf->dongle_cooldown
			- (current_time() - dongle->time_of_last_released);
	}
}



void	prepare_and_push_requests(t_coder *coder)
{
	t_config	*conf;
	t_request	req;

	conf = coder->conf;
	req.id = coder->id;
	req.enter_time = current_time();
	pthread_mutex_lock(&coder->count_mutex);
	req.time_to_burnout = coder->last_compile_time + conf->time_to_burnout;
	pthread_mutex_unlock(&coder->count_mutex);
	push_request(coder, &conf->dongles[coder->left_dongle], &req);
	push_request(coder, &conf->dongles[coder->right_dongle], &req);
}
