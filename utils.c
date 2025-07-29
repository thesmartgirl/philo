/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:01:54 by ataan             #+#    #+#             */
/*   Updated: 2025/07/29 22:27:06 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_print(t_philo *philo, char *activity, long ts)
{
	pthread_mutex_lock(&philo->sim->state_mtx);
	if (philo->sim->state == RUNNING)
	{
		pthread_mutex_lock(&philo->sim->print_mtx);
		printf("%ld %d %s\n", ts, philo->id, activity);
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

void	cleanup(t_simulation *sim, t_philo *philos, pthread_t *threads)
{
	destroy_all_mutexes(sim, philos);
	if (philos)
		free(philos);
	if (sim->forks_mtx)
		free(sim->forks_mtx);
	if (threads)
		free(threads);
}
