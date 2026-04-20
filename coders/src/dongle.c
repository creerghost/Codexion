#include "codexion.h"
#include "queue.h"
#include "utils.h"

static void	init_ticket(t_coder *coder, t_req *req)
{
	req->coder = coder;
	req->arrival_time = get_time_ms();
	req->deadline = coder->last_compile_start
		+ coder->sim->config.time_to_burnout;
	req->next = NULL;
}

static int	is_stopped(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->monitor_lock);
	stop = sim->stop_flag;
	pthread_mutex_unlock(&sim->monitor_lock);
	return (stop);
}

static int	request_and_wait(t_dongle *dongle, t_coder *coder, t_sim *sim)
{
	t_req	req;

	init_ticket(coder, &req);
	pthread_mutex_lock(&dongle->lock);
	if (is_stopped(sim))
		return (pthread_mutex_unlock(&dongle->lock), 0);
	enqueue_req(dongle, &req, sim);
	if (sim->config.scheduler == CODEX_SCHED_EDF)
		pthread_cond_broadcast(&dongle->cond);
	while (1)
	{
		if (is_stopped(sim))
			return (pthread_mutex_unlock(&dongle->lock), 0);
		if (is_turn(dongle, &req) && dongle->is_available)
		{
			if (get_time_ms() < dongle->available_at)
				usleep((dongle->available_at - get_time_ms()) * 1000);
			break ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	dongle->is_available = 0;
	return (dequeue_req(dongle), pthread_mutex_unlock(&dongle->lock), 1);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*fst;
	t_dongle	*snd;

	if (!coder->right)
		return (request_and_wait(coder->left, coder, coder->sim),
			precise_sleep(coder->sim->config.time_to_burnout + 100), 0);
	if (coder->left->id < coder->right->id)
	{
		fst = coder->left;
		snd = coder->right;
	}
	else
	{
		fst = coder->right;
		snd = coder->left;
	}
	if (!request_and_wait(fst, coder, coder->sim))
		return (0);
	if (!request_and_wait(snd, coder, coder->sim))
		return (pthread_mutex_lock(&fst->lock), fst->is_available = 1,
			pthread_cond_broadcast(&fst->cond),
			pthread_mutex_unlock(&fst->lock), 0);
	return (1);
}

void	release_dongles(t_coder *coder)
{
	long long	now;

	now = get_time_ms();
	pthread_mutex_lock(&coder->left->lock);
	coder->left->is_available = 1;
	coder->left->available_at = now + coder->sim->config.dongle_cooldown;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_mutex_unlock(&coder->left->lock);
	if (coder->right)
	{
		pthread_mutex_lock(&coder->right->lock);
		coder->right->is_available = 1;
		coder->right->available_at = now
			+ coder->sim->config.dongle_cooldown;
		pthread_cond_broadcast(&coder->right->cond);
		pthread_mutex_unlock(&coder->right->lock);
	}
}