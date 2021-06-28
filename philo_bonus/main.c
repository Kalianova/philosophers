#include "philo.h"

void	close_philo(t_all_philo *all, int num_philo)
{
	int	i;

	sem_wait(all->check_dead);
	sem_wait(all->check_dead);
	i = 0;
	while (i < num_philo)
	{
		kill(all->philos[i].pid, 15);
		i++;
	}
	sem_close(all->forks);
	sem_close(all->write);
	sem_close(all->take_fork);
	sem_close(all->check_dead);
	sem_close(all->full_eat);
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/take_fork");
	sem_unlink("/check_dead");
	sem_unlink("/full_eat");
	free(all->philos);
}

static int	init_num_philo_eat(t_info_philo *info, t_all_philo *res)
{
	int			i;
	int			pid;

	i = -1;
	if (info->num_philo_eat > 0)
	{
		while (++i < info->num_philo_eat - 1)
			sem_wait(res->full_eat);
		pid = fork();
		if (pid < 0)
			return (0);
		if (pid == 0)
		{
			monitor_dead_thread(res, info->num_philo_eat);
			exit(0);
		}
	}
	return (1);
}

static int	init_all(t_info_philo *info, t_all_philo *res)
{
	int			i;

	if (!init_num_philo_eat(info, res))
		return (0);
	i = -1;
	while (++i < info->num_philo)
	{
		res->philos[i].num_id = i;
		res->philos[i].eating = 0;
		res->philos[i].all_philo = res;
		res->philos[i].info_philo = info;
		res->philos[i].count_eat = 0;
		res->philos[i].last_eat = get_time(0);
		res->philos[i].pid = fork();
		res->philos[i].died = 0;
		if (res->philos[i].pid < 0)
			return (0);
		if (res->philos[i].pid == 0)
		{
			philo_thread(&res->philos[i]);
			exit(0);
		}
	}
	return (1);
}

static t_info_philo	init_info(int argc, char **argv)
{
	t_info_philo	res;

	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/take_fork");
	sem_unlink("/check_dead");
	sem_unlink("/full_eat");
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

	if (argc < 5 || argc > 6)
		return (print_error("Wrong number of parameters"));
	info = init_info(argc, argv);
	if (info.num_philo <= 0 || info.num_philo_eat == 0 || info.time_die < 0
		|| info.time_sleep < 0 || info.time_die < 0)
		return (print_error("Invalid parameter"));
	all.philos = (t_philo *)malloc(sizeof(t_philo) * info.num_philo);
	all.forks = sem_open("/forks", O_CREAT, 0644, info.num_philo);
	all.begin = get_time(0);
	all.take_fork = sem_open("/take_fork", O_CREAT, 0644, 1);
	all.write = sem_open("/write", O_CREAT, 0644, 1);
	all.check_dead = sem_open("/check_dead", O_CREAT, 0644, 1);
	if (info.num_philo_eat > 0)
		all.full_eat = sem_open("/full_eat", O_CREAT, 0644,
				info.num_philo_eat);
	if (!init_all(&info, &all) || all.forks == SEM_FAILED
		|| all.take_fork == SEM_FAILED || all.check_dead == SEM_FAILED
		|| all.full_eat == SEM_FAILED || all.write == SEM_FAILED)
		return (print_error("Error in semaphors or forks"));
	close_philo(&all, info.num_philo);
	return (0);
}
