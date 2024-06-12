/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 09:24:01 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/12 19:50:31 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	creat_mtr_join(t_data *data, t_monitor *monitor, t_philo *philo)
{
	int	i;

	i = 0;
	if (pthread_create(&monitor->thread_id, NULL, &monitoring, monitor) != 0)
		return (1);
	while (i < data->philos_num)
	{
		philo = data->philos + i;
		if (pthread_detach(philo->thread_id))
			return (1);
		i++;
	}
	if (pthread_join(monitor->thread_id, NULL))
		return (1);
	return (0);
}

void	creat_philos(t_data *data, t_monitor *monitor, size_t start)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = NULL;
	while (i < data->philos_num)
	{
		philo = data->philos + i;
		philo->id = i + 1;
		philo->data = data;
		philo->time_to_die = data->time_to_die;
		philo->time_to_eat = data->time_to_eat;
		philo->time_to_sleep = data->time_to_sleep;
		philo->philos_num = data->philos_num;
		philo->meals = data->meals_num;
		philo->meals_eaten = 0;
		philo->sim_start = start;
		philo->last_meal_time = philo->sim_start;
		if (pthread_create(&philo->thread_id, NULL, &philo_routine, philo) != 0)
			return ;
		i++;
	}
	if (creat_mtr_join(data, monitor, philo))
		return ;
}

int	data_alloc(t_data *data, int argc, char **argv)
{
	data->philos_num = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_num = ft_atoi(argv[5]);
	else if (argc == 5)
		data->meals_num = -1;
	data->philos = malloc(sizeof(t_philo) * data->philos_num);
	if (!data->philos)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_num);
	if (!data->forks)
		return (1);
	return (0);
}

int	check_mtx(t_data *data)
{
	int	i;

	i = -1;
	if (data->err[0] != 0)
		return (1);
	if (data->err[1] != 0)
		return (1);
	if (data->err[2] != 0)
		return (1);
	if (data->forks)
	{
		while (++i < data->philos_num)
		{
			if (data->forks_err[i] != 0)
				return (1);
		}
	}
	return (0);
}

void	init_data(t_data *data, int argc, char **argv)
{
	t_monitor	*monitor;
	int			i;

	i = -1;
	if (data_alloc(data, argc, argv))
		return ;
	monitor = malloc(sizeof(t_monitor));
	if (!monitor)
		return ;
	data->monitor = monitor;
	monitor->data = data;
	while (++i < data->philos_num)
		data->forks_err[i] = pthread_mutex_init(&data->forks[i], NULL);
	data->err[0] = pthread_mutex_init(&data->write, NULL);
	data->err[1] = pthread_mutex_init(&data->read, NULL);
	data->err[2] = pthread_mutex_init(&data->ready, NULL);
	if (check_mtx(data))
		return ;
	creat_philos(data, monitor, get_time());
}
