/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 03:11:53 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/12 19:59:31 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	sleep_r_forks(t_data *data, t_philo *philo, int count)
{
	if (pthread_mutex_lock(&data->write) != 0)
		return (1);
	printf("%zu %d is eating\n", get_time() - philo->sim_start, philo->id);
	if (pthread_mutex_unlock(&data->write) != 0)
		return (1);
	if (pthread_mutex_lock(&data->read) != 0)
		return (1);
	philo->last_meal_time = get_time();
	if (pthread_mutex_unlock(&data->read) != 0)
		return (1);
	if (pthread_mutex_lock(&data->ready) != 0)
		return (1);
	philo->meals_eaten += count;
	if (pthread_mutex_unlock(&data->ready) != 0)
		return (1);
	ft_usleep(philo->time_to_eat);
	return (0);
}

int	eat(t_philo *philo, t_data *data, int count)
{
	pthread_mutex_t	*forks;
	int				left_fork;
	int				right_fork;

	forks = data->forks;
	left_fork = philo->id - 1;
	right_fork = ((philo->id) % philo->philos_num);
	if (lock_fork(data, philo, forks + left_fork) == 1)
		return (1);
	if (lock_fork(data, philo, forks + right_fork) == 1)
		return (1);
	if (sleep_r_forks(data, philo, count) == 1)
		return (1);
	if (pthread_mutex_unlock(forks + left_fork) != 0)
		return (1);
	if (pthread_mutex_unlock(forks + right_fork) != 0)
		return (1);
	return (0);
}

int	sleep_think(t_data *data, t_philo *philo, int count)
{
	if (eat(philo, philo->data, count) == 1)
		return (1);
	if (pthread_mutex_lock(&data->write) != 0)
		return (1);
	printf("%zu %d is sleeping\n", get_time() - philo->sim_start, philo->id);
	if (pthread_mutex_unlock(&data->write) != 0)
		return (1);
	ft_usleep(philo->time_to_sleep);
	if (pthread_mutex_lock(&data->write) != 0)
		return (1);
	printf("%zu %d is thinking\n", get_time() - philo->sim_start, philo->id);
	if (pthread_mutex_unlock(&data->write) != 0)
		return (1);
	return (0);
}

void	*philo_routine(void *input)
{
	t_philo	*philo;
	t_data	*data;
	int		count;

	(1) && (count = 1, philo = (t_philo *)input, data = philo->data);
	if (philo->meals == -1)
		count = 0;
	if (philo->id % 2 != 0)
		ft_usleep(philo->time_to_eat);
	while (1 && data && philo)
	{
		if (pthread_mutex_lock(&data->ready) != 0)
			return (NULL);
		if (philo->meals_eaten == philo->meals)
		{
			if (pthread_mutex_unlock(&data->ready) != 0)
				return (NULL);
			break ;
		}
		if (pthread_mutex_unlock(&data->ready) != 0)
			return (NULL);
		if (sleep_think(data, philo, count))
			return (NULL);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data	data;

	data.forks = NULL;
	data.monitor = NULL;
	data.philos = NULL;
	if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv))
			return (1);
		init_err(&data);
		init_data(&data, argc, argv);
		destroy_mtx(&data);
		free(data.forks);
		free(data.philos);
		free(data.monitor);
	}
	return (0);
}
