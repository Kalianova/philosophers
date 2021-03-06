#include "philo.h"

static void	custom_sleep(unsigned long long wake)
{
	while (get_time(0) < wake)
		usleep(50);
}

static void	eat_lock(t_philo *philo)
{
	pthread_mutex_lock(&(philo->all_philo->forks[philo->right_fork]));
	pthread_mutex_lock(&(philo->all_philo->forks[philo->left_fork]));
	fflush(stdout);
	print_info(philo, FORK);
	print_info(philo, FORK);
	print_info(philo, EAT);
	philo->eating = 1;
	philo->last_eat = get_time(0);
	custom_sleep(get_time(0) + philo->info_philo->time_eat);
	philo->count_eat++;
	if (philo->count_eat == philo->info_philo->num_philo_eat)
	{
		philo->all_philo->count_full_eat++;
		if (philo->all_philo->count_full_eat == philo->info_philo->num_philo)
			pthread_mutex_lock(&philo->all_philo->write);
	}
	philo->eating = 0;
	pthread_mutex_unlock(&(philo->all_philo->forks[philo->right_fork]));
	pthread_mutex_unlock(&(philo->all_philo->forks[philo->left_fork]));
}

void	*monitor_dead_thread(void *arg_all)
{
	t_all_philo	*all_philo;

	all_philo = (t_all_philo *)arg_all;
	while (1)
	{
		if (all_philo->count_full_eat
			== all_philo->philos[0].info_philo->num_philo
			|| all_philo->smbd_dead)
		{
			return (NULL);
		}
		usleep(100);
	}
}

static void	*monitor_thread(void *arg_philo)
{
	t_philo	*philo;

	philo = (t_philo *)arg_philo;
	while (1)
	{
		if (!philo->eating && (int)(get_time(0) - philo->last_eat)
			> philo->info_philo->time_die)
		{
			philo->all_philo->smbd_dead = 1;
			pthread_mutex_lock(&philo->all_philo->write);
			printf("%llums %u died\n",
				get_time(philo->all_philo->begin), philo->num_id + 1);
			fflush(stdout);
			return (NULL);
		}
		usleep(100);
	}
}

void	*philo_thread(void *arg_philo)
{
	pthread_t	monitor;
	t_philo		*philo;

	philo = (t_philo *)arg_philo;
	if (philo->num_id % 2 == 1)
		usleep(50);
	pthread_create(&monitor, NULL, monitor_thread, arg_philo);
	pthread_detach(monitor);
	while (1)
	{
		eat_lock(philo);
		print_info(philo, SLEEP);
		custom_sleep(get_time(0) + philo->info_philo->time_sleep);
		print_info(philo, THINK);
	}
}
