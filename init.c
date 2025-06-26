/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:14:26 by ataan             #+#    #+#             */
/*   Updated: 2025/06/26 14:14:45 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_philos(t_simulation *sim)
{
	int		i;
	t_philo	*philos;

	philos = malloc(sizeof(t_philo) * sim->n_philos);
	if (philos == NULL)
		return (NULL);
	philos[0].forks[1] = sim->n_philos;
	philos[0].forks[0] = 1;
	i = 0;
	while (i < sim->n_philos)
	{
		philos[i].sim = sim;
		philos[i].id = i + 1;
		philos[i].s_last_meal = sim->start_time;
		philos[i].s_sleep = sim->start_time;
		philos[i].meals = 0;
		create_mutex(&philos[i].meals_mtx, sim);
		if (philos[i].id != 1)
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
	t_philo	*philos;

	gettimeofday(&sim->start_time, NULL);
	sim->state = RUNNING;
	sim->t_die = ft_atoi(argv[2]);
	sim->t_eat = ft_atoi(argv[3]);
	sim->t_sleep = ft_atoi(argv[4]);
	sim->n_must_eat = -1;
	if (argc == 6)
		sim->n_must_eat = ft_atoi(argv[5]);
	sim->mtx_count = 0;
	create_mutex(&sim->print_mtx, sim);
	create_mutex(&sim->state_mtx, sim);
	sim->forks_mtx = init_mutexes(sim->n_philos, sim);
	if (sim->forks_mtx == NULL)
		return (NULL);
	philos = init_philos(sim);
	return (philos);
}
