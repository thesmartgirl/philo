#include "philo.h"

pthread_mutex_t	*init_mutexes(int n)
{
	pthread_mutex_t	*mutexes;
	int				i;

	mutexes = malloc(sizeof(pthread_mutex_t) * n);
	if (mutexes == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(&mutexes[i], NULL) != 0)
		{
			printf("mutex_create error\n");
			// destroy any previously initialized mutexes to avoid resource leaks
			while (--i >= 0)
				pthread_mutex_destroy(&mutexes[i]);
			free(mutexes);
			return (NULL);
		}
		i++;
	}
	return (mutexes);
}

void	destroy_mutexes(pthread_mutex_t *mutexes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&mutexes[i]);
		i++;
	}
}

void	destroy_all_mutexes(t_simulation *sim, t_philo *philos)
{
	destroy_mutexes(sim->forks_mtx, sim->n_philos);
	destroy_mutexes(&sim->state_mtx, 1);
	int i = 0;
	while (i < sim->n_philos)
	{
		destroy_mutexes(&philos[i].meals_mtx, 1);
		i++;
	}
}