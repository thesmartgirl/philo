/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:15:20 by ataan             #+#    #+#             */
/*   Updated: 2025/06/26 14:15:21 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*init_mutexes(int n, t_simulation *sim)
{
	pthread_mutex_t	*mutexes;
	int				i;

	mutexes = malloc(sizeof(pthread_mutex_t) * n);
	if (mutexes == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		if (create_mutex(&mutexes[i], sim) != 0)
		{
			printf("mutex_create error\n");
			destroy_all_mutexes(sim);
			free(mutexes);
			return (NULL);
		}
		i++;
	}
	return (mutexes);
}

// void	destroy_mutexes(pthread_mutex_t *mutexes, int n)
// {
// 	int	i;

// 	i = 0;
// 	while (i < n)
// 	{
// 		pthread_mutex_destroy(&mutexes[i]);
// 		i++;
// 	}
// }

// void	destroy_all_mutexes(t_simulation *sim, t_philo *philos)
// {
// 	destroy_mutexes(sim->forks_mtx, sim->n_philos);
// 	destroy_mutexes(&sim->state_mtx, 1);
// 	int i = 0;
// 	while (i < sim->n_philos)
// 	{
// 		destroy_mutexes(&philos[i].meals_mtx, 1);
// 		i++;
// 	}
// }

int	create_mutex(pthread_mutex_t *mutex, t_simulation *sim)
{
	if (pthread_mutex_init(mutex, NULL) == 0)
	{
		sim->mutexes[sim->mtx_count++] = *mutex;
		return (0);
	}
	else
		return (1);
}

void	destroy_all_mutexes(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->mtx_count)
	{
		if (pthread_mutex_destroy(&sim->mutexes[i]) != 0)
			perror("Failed to destroy mutex");
		i++;
	}
}
