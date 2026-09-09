
#include "../codexion.h"

int	taking_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_t	*mtx;

	mtx = &dongle->available_mutex;
	pthread_mutex_lock(mtx);
	while ((!dongle->is_available || dongle->heap.size == 0
			|| dongle->heap.nodes[0].id != coder->id)
		&& !is_sim_end(coder->conf))
		pthread_cond_wait(&dongle->waiters, mtx);
	handle_cooldown(coder, dongle);
	if (is_sim_end(coder->conf))
		return (pthread_mutex_unlock(mtx), 1);
	heap_pop(&dongle->heap, coder->conf->scheduler);
	dongle->is_available = 0;
	pthread_mutex_unlock(mtx);
	print_state(coder, "has taken a dongle");
	return (0);
}

int	dongle_logic(t_coder *coder)
{
	int	first;
	int	second;

	if (coder->conf->number_of_coders == 1)
		return (ft_usleep(coder->conf->time_to_burnout, coder->conf), 1);
	prepare_and_push_requests(coder);
	first = coder->left_dongle;
	second = coder->right_dongle;
	if (coder->id % 2 == 0)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (taking_dongle(coder, &coder->conf->dongles[first]))
		return (1);
	if (taking_dongle(coder, &coder->conf->dongles[second]))
		return (release_dongle(&coder->conf->dongles[first]), 1);
	return (0);
}
