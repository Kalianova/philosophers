#include "philo.h"

static void	print_info(t_philo *philo, int type)
{
	unsigned int	timestamp_in_ms;
	unsigned int	philo_number;

	timestamp_in_ms = get_time(philo->all_philo->begin);
	philo_number = philo->num_id;
	sem_wait(philo->all_philo->write);
	printf("%ums %u ", timestamp_in_ms, philo_number + 1);
	if (type == 1)
		printf("has taken a fork\n");
	if (type == 2)
		printf("is eating\n");
	if (type == 3)
		printf("is sleeping\n");
	if (type == 4)
		printf("is thinking\n");
	fflush(stdout);
	sem_post(philo->all_philo->write);
}

static void	eat_lock(t_philo *philo)
{
	sem_wait(philo->all_philo->take_fork);
	sem_wait(philo->all_philo->forks);
	sem_wait(philo->all_philo->forks);
	sem_post(philo->all_philo->take_fork);
	fflush(stdout);
	print_info(philo, FORK);
	print_info(philo, FORK);
	print_info(philo, EAT);
	philo->eating = 1;
	philo->last_eat = get_time(0);
	custom_sleep(get_time(0) + philo->info_philo->time_eat);
	philo->count_eat++;
	if (philo->count_eat == philo->info_philo->num_philo_eat)
		sem_post(philo->all_philo->full_eat);
	philo->eating = 0;
	sem_post(philo->all_philo->forks);
	sem_post(philo->all_philo->forks);
}

void	*monitor_dead_thread(t_all_philo *all_philo, int count_philo)
{
	int			i;

	i = 0;
	while (i < count_philo - 1)
	{
		sem_wait(all_philo->full_eat);
		i++;
	}
	sem_wait(all_philo->write);
	sem_post(all_philo->check_dead);
	return (NULL);
}

static void	*monitor_thread(void *arg_philo)
{
	t_philo	*philo;

	philo = (t_philo *)arg_philo;
	while (1)
	{
		if (!philo->eating && (int)(get_time(0) - philo->last_eat)
			> philo->info_philo->time_die && !philo->died)
		{
			philo->died = 1;
			sem_wait(philo->all_philo->write);
			printf("%llums %u died\n",
				get_time(philo->all_philo->begin), philo->num_id + 1);
			fflush(stdout);
			sem_post(philo->all_philo->check_dead);
			return (NULL);
		}
		usleep(10);
	}
}

void	*philo_thread(t_philo *philo)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, monitor_thread, philo);
	pthread_detach(monitor);
	while (1)
	{
		eat_lock(philo);
		print_info(philo, SLEEP);
		custom_sleep(get_time(0) + philo->info_philo->time_sleep);
		print_info(philo, THINK);
	}
}
