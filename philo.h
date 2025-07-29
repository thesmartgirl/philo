/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:50:42 by ataan             #+#    #+#             */
/*   Updated: 2025/07/29 22:22:59 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum sim_state
{
	RUNNING,
	STOPPED,
}					t_sim_state;

typedef struct s_simulation
{
	t_sim_state		state;
	pthread_mutex_t	state_mtx;
	struct timeval	start_time;
	int				n_philos;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_must_eat;
	pthread_mutex_t	*forks_mtx;
	pthread_mutex_t	print_mtx;
}					t_simulation;

typedef struct s_philo
{
	int				id;
	int				forks[2];
	int				meals;
	struct timeval	s_last_meal;
	struct timeval	s_sleep;
	pthread_mutex_t	meals_mtx;
	t_simulation	*sim;
}					t_philo;

// ft_atoi.c
int					ft_atoi(const char *nptr);

// philo.c
void				*thread_func(void *arg);

// threads.c
pthread_t			*create_threads(int n, t_philo *philos);
void				join_threads(pthread_t *threads, int n);
pthread_t			*malloc_thread_ids(int n);

// mutex.c
pthread_mutex_t		*init_mutexes(int n);
void				destroy_mutexes(pthread_mutex_t *mutexes, int n);
void				destroy_all_mutexes(t_simulation *sim, t_philo *philos);

// init.c
t_philo				*init_philos(t_simulation *sim);
t_philo				*init_sim(int argc, char **argv, t_simulation *sim);

// utils.c
void				safe_print(t_philo *philo, char *activity, long ts);
long				t_since(struct timeval t);
void				cleanup(t_simulation *sim, t_philo *philos,
						pthread_t *threads);

// sim.c
void				run_simulation(t_simulation *sim, t_philo *philos);