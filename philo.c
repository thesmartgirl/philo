/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:15:35 by ataan             #+#    #+#             */
/*   Updated: 2025/06/26 14:18:19 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	cleanup(t_simulation *sim, t_philo *philos, pthread_t *threads)
{
	destroy_all_mutexes(sim);
	if (sim->forks_mtx)
		free(sim->forks_mtx);
	if (philos)
		free(philos);
	if (threads)
		free(threads);
}

int	main(int argc, char **argv)
{
	t_philo			*philos;
	pthread_t		*threads;
	t_simulation	sim;

	sim.n_philos = check_arg(argc, argv);
	if (sim.n_philos == -1)
		return (-1);
	philos = NULL;
	threads = NULL;
	philos = init_sim(argc, argv, &sim);
	if (philos == NULL)
	{
		cleanup(&sim, philos, threads);
		return (-1);
	}
	threads = create_threads(sim.n_philos, philos);
	if (threads == NULL)
	{
		cleanup(&sim, philos, threads);
		return (-1);
	}
	run_simulation(&sim, philos);
	join_threads(threads, sim.n_philos);
	cleanup(&sim, philos, threads);
	return (0);
}
