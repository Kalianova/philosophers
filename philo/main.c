#include "philo.h"

void	print_info(t_philo *philo, int type)
{
	unsigned int	timestamp_in_ms;
	unsigned int	philo_number;

	timestamp_in_ms = get_time(philo->all_philo->begin);
	philo_number = philo->num_id;
	pthread_mutex_lock(&philo->all_philo->write);
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
	pthread_mutex_unlock(&philo->all_philo->write);
}

static void	init_all(t_info_philo *info, t_all_philo *res)
{
	int			i;

	i = -1;
	res->philos = (t_philo *)malloc(sizeof(t_philo) * info->num_philo);
	res->forks = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * info->num_philo);
	res->begin = get_time(0);
	res->count_full_eat = 0;
	pthread_mutex_init(&res->take_fork, NULL);
	pthread_mutex_init(&res->write, NULL);
	while (++i < info->num_philo)
	{
		res->philos[i].num_id = i;
		res->philos[i].eating = 0;
		res->philos[i].right_fork = (i + 1) % info->num_philo;
		res->philos[i].left_fork = i;
		res->philos[i].all_philo = res;
		res->philos[i].info_philo = info;
		res->philos[i].count_eat = 0;
		res->philos[i].last_eat = get_time(0);
		pthread_mutex_init(&res->forks[i], NULL);
		pthread_create(&res->philos[i].thread,
			NULL, philo_thread, &res->philos[i]);
		pthread_detach(res->philos[i].thread);
	}
	pthread_create(&res->check_dead, NULL, monitor_dead_thread, res);
}

static t_info_philo	init_info(int argc, char **argv)
{
	t_info_philo	res;

	res.num_philo = ft_atoi(argv[1]);
	res.time_die = ft_atoi(argv[2]);
	res.time_eat = ft_atoi(argv[3]);
	res.time_sleep = ft_atoi(argv[4]);
	res.num_philo_eat = -1;
	if (argc == 6)
		res.num_philo_eat = ft_atoi(argv[5]);
	return (res);
}

int	main(int argc, char **argv)
{
	t_info_philo	info;
	t_all_philo		all;
	int				i;
// ОБРАБОТКА ОШИБОК МЬЮТЕКСОВ
	if (argc < 5 || argc > 6)
		return (print_error("Wrong number of parameters"));
	info = init_info(argc, argv);
	if (info.num_philo <= 0 || info.num_philo_eat == 0 || info.time_die < 0
		|| info.time_sleep < 0 || info.time_die < 0)
		return (print_error("Invalid parameter"));
	init_all(&info, &all);
	all.smbd_dead = 0;
	pthread_join(all.check_dead, NULL);
	pthread_detach(all.check_dead);
	i = 0;
	while (i < info.num_philo)
	{
		pthread_mutex_destroy(&all.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&all.write);
	pthread_mutex_destroy(&all.take_fork);
	free (all.philos);
	free (all.forks);
	return (0);
}
