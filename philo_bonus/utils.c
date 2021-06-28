#include "philo.h"

unsigned long long	get_time(unsigned long long last_time)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_usec / 1000 + time.tv_sec * 1000 - last_time);
}

void	custom_sleep(unsigned long long wake)
{
	while (get_time(0) < wake)
		usleep(50);
}

int	ft_atoi(const char *nptr)
{
	int			i;
	int			k;
	long long	res;

	i = 0;
	k = 1;
	res = 0;
	if (nptr[i] == '-')
		return (-1);
	while (res <= 2147483648 && nptr[i] && nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	if (res > 2147483647 || nptr[i] != '\0')
		return (-1);
	else
		return (res);
}

int	print_error(char *msg)
{
	printf("%s\n", msg);
	fflush(stdout);
	return (0);
}
