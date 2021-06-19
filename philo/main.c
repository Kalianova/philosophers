#include "philo.h"
#include <stdio.h>

void	print_info(unsigned int timestamp_in_ms,
unsigned int philo_number, int type)
{
	printf("%ums %u ", timestamp_in_ms, philo_number);
	if (type == 1)
		printf("has taken a fork\n");
	if (type == 2)
		printf("is eating\n");
	if (type == 3)
		printf("is sleeping\n");
	if (type == 4)
		printf("is thinking\n");
	if (type == 5)
		printf("died\n");
	fflush(stdout);
}

int	main(int argc, char **argv)
{
	t_philo_info	info;

	print_info(argc, argc, 1);
	return (0);
}
