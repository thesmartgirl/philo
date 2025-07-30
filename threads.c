/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:02:05 by ataan             #+#    #+#             */
/*   Updated: 2025/07/30 20:45:36 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_t	*create_threads(int n, t_philo *philos)
{
	pthread_t	*threads;
	int			i;

	threads = malloc_thread_ids(n);
	if (threads == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		if (pthread_create(&threads[i], NULL, thread_func, &philos[i]) != 0)
		{
			pthread_mutex_lock(&philos[0].sim->print_mtx);
			printf("pthread create error\n");
			pthread_mutex_unlock(&philos[0].sim->print_mtx);
			pthread_mutex_lock(&philos[0].sim->state_mtx);
			philos[0].sim->state = STOPPED;
			pthread_mutex_unlock(&philos[0].sim->state_mtx);
			join_threads(threads, i);
			cleanup(philos[0].sim, philos, threads);
			return (NULL);
		}
		i++;
	}
	return (threads);
}

void	join_threads(pthread_t *threads, int n)
{
	int	i;

	if (threads == NULL)
		return ;
	i = 0;
	while (i < n)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

pthread_t	*malloc_thread_ids(int n)
{
	pthread_t	*threads;

	threads = malloc(sizeof(pthread_t) * n);
	if (!threads)
	{
		printf("malloc error\n");
		return (NULL);
	}
	return (threads);
}
