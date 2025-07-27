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

void p_eat(t_philo *p)
{
	int f1;
	int f2;

	f1 = p->forks[0] - 1;
	f2 = p->forks[1] - 1;

	// printf("forks: %d, %d\n", f1, f2);
	// if( f1 != f2)
	// {
		pthread_mutex_lock(&p->sim->forks_mtx[f1]);
		safe_print(p, "has taken a fork", t_since(p->sim->start_time));
		pthread_mutex_lock(&p->sim->forks_mtx[f2]);
		safe_print(p, "has taken a fork", t_since(p->sim->start_time));
		safe_print(p, "is eating", t_since(p->sim->start_time));
		pthread_mutex_lock(&p->meals_mtx);
		p->meals++;
		gettimeofday(&p->s_last_meal, NULL);
		pthread_mutex_unlock(&p->meals_mtx);
		// printf("t_eat = %d\n", p->sim->t_eat);
		short_naps(p, p->sim->t_eat);
		// printf("philo %d finished eating at %ld\n", p->id, t_since(p->sim->start_time));
		pthread_mutex_unlock(&p->sim->forks_mtx[f2]);
		pthread_mutex_unlock(&p->sim->forks_mtx[f1]);
	// }
}

void p_sleep(t_philo *p)
{
	safe_print(p, "is sleeping", t_since(p->sim->start_time));
	short_naps(p, p->sim->t_sleep);
}

void p_think(t_philo *p)
{
	safe_print(p, "is thinking", t_since(p->sim->start_time));
}

void	*thread_func(void *arg)
{
	t_philo	*p;
	// long	time;
	// int		sim_state;

	p = (t_philo *)arg;
	while (1)
	{
		if(p->id % 2 == 0)
			usleep(500);
		pthread_mutex_lock(&p->sim->state_mtx);
		if(p->sim->state != RUNNING)
		{
			pthread_mutex_unlock(&p->sim->state_mtx);
			return NULL;
		}
		pthread_mutex_unlock(&p->sim->state_mtx);
		p_eat(p);
		p_sleep(p);
		p_think(p);
    }
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
