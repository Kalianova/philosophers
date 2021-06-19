#ifndef PHILO_H
# define PHILO_H

typedef struct	s_philo_info
{
	unsigned int number_of_philosophers;
	unsigned int time_to_die;
	unsigned int time_to_eat;
	unsigned int time_to_sleep;
	unsigned int number_of_times_each_philosopher_must_eat;
}				t_philo_info;

# endif PHILO_H