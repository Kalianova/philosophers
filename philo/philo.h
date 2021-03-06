#ifndef PHILO_H
# define PHILO_H
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4

struct	s_all_philo;
struct	s_info_philo;

typedef struct s_philo
{
	int						eating;
	int						right_fork;
	int						left_fork;
	int						num_id;
	int						count_eat;
	unsigned long long		last_eat;
	pthread_t				thread;
	struct s_all_philo		*all_philo;
	struct s_info_philo		*info_philo;		
}				t_philo;

typedef struct s_info_philo
{
	int	num_philo;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	num_philo_eat;
}				t_info_philo;

typedef struct s_all_philo
{
	int					smbd_dead;
	int					count_full_eat;
	unsigned long long	begin;
	pthread_mutex_t		*forks;
	pthread_mutex_t		write;
	pthread_t			check_dead;
	t_philo				*philos;
}				t_all_philo;

unsigned long long	get_time(unsigned long long last_time);
int					ft_atoi(const char *nptr);
int					print_error(char *msg);
void				*philo_thread(void *arg_philo);
void				*monitor_dead_thread(void *arg_all);
void				print_info(t_philo *philo, int type);

#endif