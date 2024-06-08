/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 03:11:53 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/04 09:48:11 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleep_r_forks(t_data *data, t_philo *philo, int count)
{
	pthread_mutex_lock(&data->write);
	printf("%zu %d is eating\n", get_time() - philo->sim_start, philo->id);
	pthread_mutex_unlock(&data->write);
	pthread_mutex_lock(&data->read);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&data->read);
	pthread_mutex_lock(&data->ready);
	philo->meals_eaten += count;
	pthread_mutex_unlock(&data->ready);
	ft_usleep(philo->time_to_eat);
}

void	eat(t_philo *philo, t_data *data, int count)
{
	pthread_mutex_t	*forks;
	int				left_fork;
	int				right_fork;

	forks = data->forks;
	left_fork = philo->id - 1;
	right_fork = ((philo->id) % philo->philos_num);
	pthread_mutex_lock(forks + left_fork);
	pthread_mutex_lock(&data->write);
	printf("%zu %d has taken a fork\n",
		get_time() - philo->sim_start, philo->id);
	pthread_mutex_unlock(&data->write);
	pthread_mutex_lock(forks + right_fork);
	pthread_mutex_lock(&data->write);
	printf("%zu %d has taken a fork\n",
		get_time() - philo->sim_start, philo->id);
	pthread_mutex_unlock(&data->write);
	sleep_r_forks(data, philo, count);
	if (pthread_mutex_unlock(forks + left_fork) != 0)
		return ;
	if (pthread_mutex_unlock(forks + right_fork) != 0)
		return ;
}

int	sleep_think(t_data *data, t_philo *philo, int count)
{
	eat(philo, philo->data, count);
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

	count = 1;
	philo = (t_philo *)input;
	data = philo->data;
	if (philo->meals == -1)
		count = 0;
	if (philo->id % 2 != 0)
		ft_usleep(philo->time_to_eat);
	while (1)
	{
		pthread_mutex_lock(&data->ready);
		if (philo->meals_eaten == philo->meals)
		{
			pthread_mutex_unlock(&data->ready);
			break ;
		}
		pthread_mutex_unlock(&data->ready);
		if (sleep_think(data, philo, count))
			return (NULL);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv))
			return (free(data), 1);
		init_data(data, argc, argv);
		destroy_mtx(data);
	}
	free(data->forks);
	free(data->philos);
	free(data->monitor);
	free(data);
	return (0);
}
