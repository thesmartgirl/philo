#include "philo.h"

int	check_sim_state(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->state_mtx);
	if (philo->sim->state != RUNNING)
	{
		pthread_mutex_unlock(&philo->sim->state_mtx);
		return (0);
	}
	pthread_mutex_unlock(&philo->sim->state_mtx);
	return (1);
}

void	safe_print(t_philo *philo, char *activity, long ts)
{
	pthread_mutex_lock(&philo->sim->state_mtx);
	if(philo->sim->state == RUNNING)
	{
		pthread_mutex_lock(&philo->sim->print_mtx);
		printf("timestamp_in_ms %ldms %d %s\n", ts, philo->id, activity);
		pthread_mutex_unlock(&philo->sim->print_mtx);
	}
	pthread_mutex_unlock(&philo->sim->state_mtx);
}

long	t_since(struct timeval t)
{
	struct timeval	now;
	long			seconds;
	long			useconds;

	gettimeofday(&now, NULL);
	seconds = now.tv_sec - t.tv_sec;
	useconds = now.tv_usec - t.tv_usec;
	return (seconds * 1000 + useconds / 1000);
}

