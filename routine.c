/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:17:48 by ataan             #+#    #+#             */
/*   Updated: 2025/06/26 14:18:05 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Sleep in short intervals to check for STOPPED
static void	short_naps(t_philo *p, int ms)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (t_since(start) < ms)
	{
		usleep(500);
		pthread_mutex_lock(&p->sim->state_mtx);
		if (p->sim->state != RUNNING)
		{
			pthread_mutex_unlock(&p->sim->state_mtx);
			break ;
		}
		pthread_mutex_unlock(&p->sim->state_mtx);
	}
}

void	p_eat(t_philo *p)
{
	int	f1;
	int	f2;

	f1 = p->forks[0] - 1;
	f2 = p->forks[1] - 1;
	pthread_mutex_lock(&p->sim->forks_mtx[f1]);
	safe_print(p, "has taken a fork", t_since(p->sim->start_time));
	pthread_mutex_lock(&p->sim->forks_mtx[f2]);
	safe_print(p, "has taken a fork", t_since(p->sim->start_time));
	safe_print(p, "is eating", t_since(p->sim->start_time));
	pthread_mutex_lock(&p->meals_mtx);
	p->meals++;
	gettimeofday(&p->s_last_meal, NULL);
	pthread_mutex_unlock(&p->meals_mtx);
	short_naps(p, p->sim->t_eat);
	pthread_mutex_unlock(&p->sim->forks_mtx[f2]);
	pthread_mutex_unlock(&p->sim->forks_mtx[f1]);
}

void	p_sleep(t_philo *p)
{
	safe_print(p, "is sleeping", t_since(p->sim->start_time));
	short_naps(p, p->sim->t_sleep);
}

void	p_think(t_philo *p)
{
	safe_print(p, "is thinking", t_since(p->sim->start_time));
}

void	*thread_func(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&p->sim->state_mtx);
		if (p->sim->state != RUNNING)
		{
			pthread_mutex_unlock(&p->sim->state_mtx);
			return (NULL);
		}
		pthread_mutex_unlock(&p->sim->state_mtx);
		if (p->sim->n_philos == 1)
		{
			pthread_mutex_lock(&p->sim->forks_mtx[0]);
			safe_print(p, "has taken a fork", t_since(p->sim->start_time));
			short_naps(p, p->sim->t_die);
			pthread_mutex_unlock(&p->sim->forks_mtx[0]);
			return (NULL);
		}
		p_eat(p);
		p_sleep(p);
		p_think(p);
	}
}
