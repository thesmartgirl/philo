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
			printf("pthread_create error\n");
			free(threads);
			return (NULL);
		}
		i++;
	}
	return (threads);
}

void	join_threads(pthread_t *threads, int n)
{
	int	i;

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
