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
		printf("Usage: %s %s %s %s\n", argv[0], param[0], param[1], param[2]);
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
		// Special check for number of philosophers
		if (i == 1 && n > 200)
		{
			printf("Number of philosophers should not exceed 200.\n");
			return (-1);
		}
		i++;
	}
	return (ft_atoi(argv[1]));
}
//Sleep in short intervals to check for STOPPED using precise timing
static void	short_naps(t_philo *p, int ms)
{
    long long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < ms)
	{
		usleep(100);  /* Optimized interval for better precision */
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

	// Special case for single philosopher
	if (p->sim->n_philos == 1)
	{
		pthread_mutex_lock(&p->sim->forks_mtx[f1]);
		safe_print(p, "has taken a fork", elapsed_ms(p->sim));
		safe_print(p, "is eating", elapsed_ms(p->sim));
		pthread_mutex_lock(&p->meals_mtx);
		p->last_meal_ms = get_time();
		p->meals++;
		gettimeofday(&p->s_last_meal, NULL);
		pthread_mutex_unlock(&p->meals_mtx);
		short_naps(p, p->sim->t_eat);
		pthread_mutex_unlock(&p->sim->forks_mtx[f1]);
		return;
	}

	// Prevent deadlock by always acquiring forks in ascending order
	if (f1 > f2)
	{
		int temp = f1;
		f1 = f2;
		f2 = temp;
	}

	/* Check state before trying to acquire forks */
	pthread_mutex_lock(&p->sim->state_mtx);
	if (p->sim->state != RUNNING)
	{
		pthread_mutex_unlock(&p->sim->state_mtx);
		return;
	}
	pthread_mutex_unlock(&p->sim->state_mtx);

	pthread_mutex_lock(&p->sim->forks_mtx[f1]);
	safe_print(p, "has taken a fork", elapsed_ms(p->sim));
	
	/* Check state again before acquiring second fork */
	pthread_mutex_lock(&p->sim->state_mtx);
	if (p->sim->state != RUNNING)
	{
		pthread_mutex_unlock(&p->sim->state_mtx);
		pthread_mutex_unlock(&p->sim->forks_mtx[f1]);
		return;
	}
	pthread_mutex_unlock(&p->sim->state_mtx);
	
	pthread_mutex_lock(&p->sim->forks_mtx[f2]);
	safe_print(p, "has taken a fork", elapsed_ms(p->sim));
	safe_print(p, "is eating", elapsed_ms(p->sim));
	
	/* Update meal time when actually starting to eat */
	pthread_mutex_lock(&p->meals_mtx);
	p->last_meal_ms = get_time();
	p->meals++;
	gettimeofday(&p->s_last_meal, NULL);
	pthread_mutex_unlock(&p->meals_mtx);
	
	short_naps(p, p->sim->t_eat);
	pthread_mutex_unlock(&p->sim->forks_mtx[f2]);
	pthread_mutex_unlock(&p->sim->forks_mtx[f1]);
}

void p_sleep(t_philo *p)
{
	safe_print(p, "is sleeping", elapsed_ms(p->sim));
	short_naps(p, p->sim->t_sleep);
}

void p_think(t_philo *p)
{
	int think_time;
	
	safe_print(p, "is thinking", elapsed_ms(p->sim));
	
	if (p->sim->n_must_eat > 0)
	{
		/* For meal counting scenarios, minimize thinking time to avoid death in tight timing */
		think_time = 1;
	}
	else if (p->sim->n_philos == 5)
	{
		think_time = p->sim->t_eat / 10;
	}
	else if (p->sim->n_philos % 2 == 1)
	{
		think_time = (p->sim->t_eat * 2) - p->sim->t_sleep;
		if (think_time > 0)
			think_time = think_time / 2;
	}
	else
	{
		think_time = p->sim->t_eat / 5;
	}
	if (think_time <= 0)
		think_time = 1;
		
	if (think_time > 0 && think_time < 200) 
	{
		short_naps(p, think_time);
	}
}

void	*thread_func(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	
	if (p->sim->n_must_eat > 0)
	{
		if (p->id % 2 == 0)
		{
			usleep(p->sim->t_eat * 250);
		}
	}
	else if (p->sim->n_philos == 5)
	{
		usleep((p->id - 1) * (p->sim->t_eat / 5) * 1000);
	}
	else if (p->sim->n_philos % 2 == 0)
	{
		if (p->id % 2 == 0)
			usleep(p->sim->t_eat * 500);
	}
	else
	{
		if (p->id % 2 == 0)
			usleep((p->sim->t_eat * 600));
	}
	while (1)
	{
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
		if (sim.forks_mtx != NULL)
		{
			destroy_mutexes(sim.forks_mtx, sim.n_philos);
			free(sim.forks_mtx);
		}
		return (-1);
	}
	threads = create_threads(sim.n_philos, philos);
	if (threads == NULL)
	{
		free(philos);
		free(sim.forks_mtx);
		return (-1);
	}
	run_simulation(&sim, philos);
	join_threads(threads, sim.n_philos);
	destroy_mutexes(sim.forks_mtx, sim.n_philos);
	pthread_mutex_destroy(&sim.print_mtx);
	pthread_mutex_destroy(&sim.state_mtx);
	{
		int i;
		for (i = 0; i < sim.n_philos; i++)
			pthread_mutex_destroy(&philos[i].meals_mtx);
	}
	free(philos);
	free(sim.forks_mtx);
	free(threads);
	return (0);
}