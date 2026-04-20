#include "codexion.h"

static void free_all(t_sim *sim, int coders, int dongles,
	int m_print, int m_lock)
{
	if (m_print)
		pthread_mutex_destroy(&sim->print_lock);
	if (m_lock)
		pthread_mutex_destroy(&sim->monitor_lock);
	if (coders)
		free(sim->coders);
	if (dongles)
		free(sim->dongles);
}


static void	destroy_dongles(t_sim *sim, int max)
{
	int	j;

	j = 0;
	while (j < max)
	{
		pthread_mutex_destroy(&sim->dongles[j].lock);
		pthread_cond_destroy(&sim->dongles[j].cond);
		j++;
	}
}

static int	init_dongles_mutexes(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].is_available = 1;
		sim->dongles[i].available_at = 0;
		sim->dongles[i].queue_head = NULL;
		if (pthread_mutex_init(&sim->dongles[i].lock, NULL) != 0)
			return (destroy_dongles(sim, i), 0);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i].lock);
			return (destroy_dongles(sim, i), 0);
		}
		i++;
	}
	return (1);
}

static void	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1)
			% sim->config.number_of_coders];
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].compile_count = 0;
		sim->coders[i].sim = sim;
		if (sim->coders[i].left == sim->coders[i].right)
			sim->coders[i].right = NULL;
		i++;
	}
}

int	init_sim(t_sim *sim, t_config *config)
{
	sim->config = *config;
	sim->coders = (t_coder *)malloc(sizeof(t_coder) * sim->config.number_of_coders);
	if (!sim->coders)
		return (0);
	sim->dongles = (t_dongle *)malloc(sizeof(t_dongle) * sim->config.number_of_coders);
	if (!sim->dongles)
		return (free_all(sim, 1, 0, 0, 0), 0);
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (free_all(sim, 1, 1, 0, 0), 0);
	if (pthread_mutex_init(&sim->monitor_lock, NULL) != 0)
		return (free_all(sim, 1, 1, 1, 0), 0);
	if (!init_dongles_mutexes(sim))
	{
		free_all(sim, 1, 1, 1, 1);
		return (0);
	}
	init_coders(sim);
	sim->stop_flag = 0;
	return (1);
}
