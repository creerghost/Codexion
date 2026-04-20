#include "codexion.h"
#include "coder.h"
#include "monitor.h"
#include "utils.h"
#include <stdlib.h>

static void	cleanup_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->monitor_lock);
	free(sim->coders);
	free(sim->dongles);
}

static void	join_and_clean(t_sim *sim, pthread_t monitor)
{
	int	i;

	pthread_join(monitor, NULL);
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	cleanup_sim(sim);
}

int	start_sim(t_sim *sim)
{
	int			i;
	pthread_t	monitor_thread;
	long long	start_time;

	start_time = get_time_ms();
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].last_compile_start = start_time;
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				&sim->coders[i]) != 0)
			return (0);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, sim) != 0)
		return (0);
	join_and_clean(sim, monitor_thread);
	return (1);
}