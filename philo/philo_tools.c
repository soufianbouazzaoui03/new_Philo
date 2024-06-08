/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 04:52:09 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/04 11:44:32 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tv;
	size_t			time;

	if (gettimeofday(&tv, NULL))
		write(2, "gettimeofday error\n", 20);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}

void	ft_usleep(size_t time_ms)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < time_ms)
		usleep(200);
}

int	check_args(int argc, char **argv)
{
	int	num;

	num = ft_atoi(argv[1]);
	if (num < 0 || num > 200)
		return (1);
	num = ft_atoi(argv[2]);
	if (num < 60)
		return (1);
	num = ft_atoi(argv[3]);
	if (num < 60)
		return (1);
	num = ft_atoi(argv[4]);
	if (num < 60)
		return (1);
	if (argc == 6)
	{
		num = ft_atoi(argv[5]);
		if (num < 0)
			return (1);
	}
	return (0);
}

void	destroy_mtx(t_data *data)
{
	int	i;

	i = -1;
	if (!data->err[0])
		pthread_mutex_destroy(&data->write);
	if (!data->err[1])
		pthread_mutex_destroy(&data->read);
	if (!data->err[2])
		pthread_mutex_destroy(&data->ready);
	if (data->forks)
	{
		while (++i < data->philos_num)
		{
			if (!data->forks_err[i])
				pthread_mutex_destroy(&data->forks[i]);
		}
	}
}
