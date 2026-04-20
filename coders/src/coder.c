#include "codexion.h"
#include "utils.h"
#include "dongle.h"
#include <stdio.h>

static void	print_action(t_coder *coder, char *action)
{
	pthread_mutex_lock(&coder->sim->print_lock);
	pthread_mutex_lock(&coder->sim->monitor_lock);
	if (coder->sim->stop_flag)
	{
		pthread_mutex_unlock(&coder->sim->monitor_lock);
		pthread_mutex_unlock(&coder->sim->print_lock);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->monitor_lock);
	printf("Coder %d %s\n", coder->id, action);	
	pthread_mutex_unlock(&coder->sim->print_lock);
}


void	*coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 != 0)
		precise_sleep(coder->sim->config.time_to_compile / 2);
	while (1)
	{
		print_action(coder, "is refactoring");
		precise_sleep(coder->sim->config.time_to_refactor);
		if (!take_dongles(coder))
			break;
		pthread_mutex_lock(&coder->sim->monitor_lock);
		coder->last_compile_start = get_time_ms();
		pthread_mutex_unlock(&coder->sim->monitor_lock);
		print_action(coder, "is compiling");
		precise_sleep(coder->sim->config.time_to_compile);
		release_dongles(coder);
		pthread_mutex_lock(&coder->sim->monitor_lock);
		coder->compile_count++;
		pthread_mutex_unlock(&coder->sim->monitor_lock);
		print_action(coder, "is debugging");
		precise_sleep(coder->sim->config.time_to_debug);
		if (coder->compile_count == coder->sim->config.number_of_compiles_required)
			break;
	}
	return (NULL);
}