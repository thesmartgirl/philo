#include "philo.h"

//Argument Checks
int	check_arg(int argc, char **argv)
{
	int		n;
	int		i;
	char	*param[3];

	if (argc != 5 && argc != 6)
	{
		param[0] = "<number_of_philosophers>";
		param[1] = "<time_to_die> <time_to_eat> <time_to_sleep>";
		param[2] = "[number_of_times_each_philosopher_must_eat]";
		printf("Usage: %s %s %s %s\n", param[0], param[1], param[2], argv[0]);
		return (-1);
	}
	i = 1;
	while (argv[i])
	{
		n = ft_atoi(argv[i]);
		if (n <= 0)
		{
			printf("Please provide a positive integer.\n");
			return (-1);
		}
		i++;
	}
	return (ft_atoi(argv[1]));
}

int	main(int argc, char **argv)
{
	t_philo			*philos;
	pthread_t		*threads;
	t_simulation	sim;

	sim.n_philos = check_arg(argc, argv);
	if (sim.n_philos == -1)
		return (-1);
	philos = init_sim(argc, argv, &sim);
	if (philos == NULL)
	{
		//destroy print mutex
		//destroy state mutex
		// destroy_mutexes(sim.forks_mtx, sim.n_philos);
		free(sim.forks_mtx);
		return (-1);
	}
	threads = create_threads(sim.n_philos, philos);
	if (threads == NULL)
	{
		free(philos);
		//destroy print mutex
		//destroy state mutex
		// destroy_mutexes(sim.forks_mtx, sim.n_philos);
		free(sim.forks_mtx);
		return (-1);
	}
	run_simulation(&sim, philos);
	join_threads(threads, sim.n_philos);
	// destroy_mutexes(sim.forks_mtx, sim.n_philos);
	free(philos);
	free(sim.forks_mtx);
	free(threads);
	return (0);
}
