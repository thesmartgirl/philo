/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:00:15 by ataan             #+#    #+#             */
/*   Updated: 2025/07/29 22:23:18 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	numeric_check(char *s)
{
	int	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i] != '\0')
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

// Argument Checks
int	check_arg(int argc, char **argv)
{
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
		if (!numeric_check(argv[i]) || ft_atoi(argv[i]) <= 0)
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
	if (sim.n_philos == 1)
	{
		printf("%s 1 died\n", argv[2]);
		return (0);
	}
	philos = init_sim(argc, argv, &sim);
	if (philos == NULL)
		cleanup(&sim, philos, NULL);
	threads = create_threads(sim.n_philos, philos);
	if (threads == NULL)
		cleanup(&sim, philos, threads);
	run_simulation(&sim, philos);
	join_threads(threads, sim.n_philos);
	cleanup(&sim, philos, threads);
	return (0);
}
