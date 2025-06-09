#include "philo.h"

int	check_sim_state(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->state_mtx);
	if (philo->sim->state != RUNNING)
	{
		pthread_mutex_unlock(&philo->sim->state_mtx);
		return (0);
	}
	pthread_mutex_unlock(&philo->sim->state_mtx);
	return (1);
}

void	safe_print(t_philo *philo, char *activity, long long ts)
{
	pthread_mutex_lock(&philo->sim->state_mtx);
	if(philo->sim->state == RUNNING)
	{
		pthread_mutex_lock(&philo->sim->print_mtx);
		printf("timestamp_in_ms %lldms %d %s\n", ts, philo->id, activity);
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

/**
 * @name get_time
 * @brief Gets the current time in milliseconds
 *
 * @return long long Current time in milliseconds since epoch
 *
 * Example:
 * ┌─────────────────────────────────────────────────┐
 * │ Time Calculation:                               │
 * │                                                 │
 * │ seconds × 1000 + microseconds ÷ 1000 = millisec │
 * │                                                 │
 * │ 1 second = 1000 milliseconds                    │
 * │ 1000 microseconds = 1 millisecond               │
 * └─────────────────────────────────────────────────┘
 */
long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @name precise_sleep
 * @brief Sleeps for a specified amount of time with high precision
 *
 * @param time_in_ms Time to sleep in milliseconds
 *
 * Example:
 * ┌─────────────────────────────────────────────────┐
 * │ Precision Sleep Implementation:                 │
 * │                                                 │
 * │ 1. Record start time                            │
 * │ 2. Loop until current_time - start ≥ target     │
 * │ 3. Small usleep(100) to avoid CPU hogging       │
 * │                                                 │
 * │ More precise than standard sleep functions      │
 * └─────────────────────────────────────────────────┘
 */
void	precise_sleep(long long time_in_ms)
{
	long long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(100);
}

/**
 * @name elapsed_ms
 * @brief Calculates elapsed time in milliseconds since simulation start
 * 
 * @param sim Pointer to simulation structure
 * @return long long Elapsed milliseconds since simulation start
 * 
 * This function provides consistent timing calculations using the precise
 * millisecond-based timing system instead of the older timeval approach.
 */
long long	elapsed_ms(t_simulation *sim)
{
	return (get_time() - sim->start_time_ms);
}

