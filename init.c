#include "philo.h"

t_philo	*init_philos(t_simulation *sim)
{
	int		i;
	t_philo	*philos;

	philos = malloc(sizeof(t_philo) * sim->n_philos);
	if (philos == NULL)
		return (NULL);
	i = 0;
	while (i < sim->n_philos)
	{
		philos[i].sim = sim;
		philos[i].id = i + 1;
		philos[i].s_last_meal = sim->start_time;
		philos[i].s_sleep = sim->start_time;
		philos[i].meals = 0;
		pthread_mutex_init(&philos[i].meals_mtx, NULL); // per-philo mutex
		//assign
		philos[0].forks[1] = philos[i].sim->n_philos;
		philos[0].forks[0] = 1;
		if(philos[i].id != 1)
		{
			philos[i].forks[0] = philos[i].id - 1;
			philos[i].forks[1] = philos[i].id;
		}
		i++;
	}
	return (philos);
}

t_philo	*init_sim(int argc, char **argv, t_simulation *sim)
{
	t_philo *philos;

	gettimeofday(&sim->start_time, NULL);
	sim->state = RUNNING;
	sim->t_die = ft_atoi(argv[2]); //these should be checked in arg check too
	sim->t_eat = ft_atoi(argv[3]);
	sim->t_sleep = ft_atoi(argv[4]);
	sim->n_must_eat = -1;
	if (argc == 6)
		sim->n_must_eat = ft_atoi(argv[5]);
	pthread_mutex_init(&sim->print_mtx, NULL);
	sim->forks_mtx = init_mutexes(sim->n_philos);
	pthread_mutex_init(&sim->state_mtx, NULL);
	if (sim->forks_mtx == NULL)
	{
		pthread_mutex_destroy(&sim->print_mtx);
		pthread_mutex_destroy(&sim->state_mtx);
		return (NULL);
	}
	philos = init_philos(sim);
	return (philos);
}
