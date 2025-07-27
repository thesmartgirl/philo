#include "philo.h"

static int	enough_eating(t_simulation *sim, t_philo *philos)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (sim->n_must_eat == -1)
		return 0;
	while (i < sim->n_philos)
	{
		pthread_mutex_lock(&philos[i].meals_mtx);
		if (philos[i].meals >= sim->n_must_eat)
			count++;
		pthread_mutex_unlock(&philos[i].meals_mtx);
		i++;
	}
	if (count >= sim->n_philos)
	{
		printf("enough meals for everyone\n");
		return (1);
	}
	else
		return (0);
}

static int	any_ph_dead(t_simulation *sim, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < sim->n_philos)
	{
		pthread_mutex_lock(&philos[i].meals_mtx);
		if (t_since(philos[i].s_last_meal) >= sim->t_die)
		{
			pthread_mutex_unlock(&philos[i].meals_mtx);
			safe_print(&philos[i], "died", t_since(sim->start_time));
			return (1);
		}
		pthread_mutex_unlock(&philos[i].meals_mtx);
		i++;
	}
	return (0);
}

/*
	if any dead philo -> break
	if enough eating -> break
	only place to set sim.state = STOPPED
*/
void	run_simulation(t_simulation *sim, t_philo *philos)
{
	while (1)
	{
		if(any_ph_dead(sim, philos) || enough_eating(sim, philos))
		{
			pthread_mutex_lock(&sim->state_mtx);
			sim->state= STOPPED;
			pthread_mutex_unlock(&sim->state_mtx);
			break;
		}
		usleep(1000);  // Add this to prevent busy loop & reduce starvation
	}
}
