#include "codexion.h"
#include "utils.h"
#include <stdio.h>

static void	trigger_stop(t_sim *sim)
{
	int	d;

	pthread_mutex_lock(&sim->monitor_lock);
	sim->stop_flag = 1;
	pthread_mutex_unlock(&sim->monitor_lock);
	d = 0;
	while (d < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[d].lock);
		pthread_cond_broadcast(&sim->dongles[d].cond);
		pthread_mutex_unlock(&sim->dongles[d].lock);
		d++;
	}
}

static int	check_burnout(t_sim *sim, t_coder *coder)
{
	long long	elapsed;

	pthread_mutex_lock(&sim->monitor_lock);
	elapsed = get_time_ms() - coder->last_compile_start;
	pthread_mutex_unlock(&sim->monitor_lock);
	if (elapsed >= sim->config.time_to_burnout)
	{
		trigger_stop(sim);
		pthread_mutex_lock(&sim->print_lock);
		printf("Coder %d burned out\n", coder->id);
		pthread_mutex_unlock(&sim->print_lock);
		return (1);
	}
	return (0);
}

static int	check_all_finished(t_sim *sim)
{
	int	i;
	int	finished;

	if (sim->config.number_of_compiles_required == -1)
		return (0);
	i = 0;
	finished = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->monitor_lock);
		if (sim->coders[i].compile_count
			>= sim->config.number_of_compiles_required)
			finished++;
		pthread_mutex_unlock(&sim->monitor_lock);
		i++;
	}
	if (finished == sim->config.number_of_coders)
	{
		trigger_stop(sim);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (1)
	{
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			if (check_burnout(sim, &sim->coders[i]))
				return (NULL);
			i++;
		}
		if (check_all_finished(sim))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
