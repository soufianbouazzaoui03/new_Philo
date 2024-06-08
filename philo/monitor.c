/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 09:07:08 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/04 09:47:58 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_meals(t_philo *philos, int num, int meals, t_data *data)
{
	int	i;

	i = 0;
	if (philos->philos_num == 1)
		return (0);
	while (i < num)
	{
		if (pthread_mutex_lock(&data->ready))
			return (write(2, "mtx_error\n", 11), 1);
		if (philos[i].meals_eaten != meals || philos[i].meals == -1)
		{
			if (pthread_mutex_unlock(&data->ready))
				return (write(2, "mtx_error\n", 11), 1);
			return (0);
		}
		else
		{
			if (pthread_mutex_unlock(&data->ready))
				return (write(2, "mtx_error\n", 11), 1);
		}
		i++;
	}
	return (1);
}

void	ft_died(t_data *data, t_philo *philo)
{
	if (pthread_mutex_lock(&data->write) != 0)
		write(2, "mtx_error\n", 11);
	printf("%lu %d died\n", (get_time() - philo->sim_start), philo->id);
	data->death = true;
}

void	*monitoring(void *input)
{
	t_monitor	*monitor;
	t_data		*data;
	t_philo		*philos;
	size_t		left;
	int			i;

	(1) && (i = 0, monitor = (t_monitor *)input, data = monitor->data,
		data->lock_error = 0, philos = data->philos);
	while (!data->death)
	{
		if (i == data->philos_num)
			i = 0;
		data->lock_error = pthread_mutex_lock(&data->read);
		left = get_time() - philos[i].last_meal_time;
		data->lock_error = pthread_mutex_unlock(&data->read);
		if (check_meals(philos, data->philos_num,
				data->meals_num, data) || data->lock_error)
			return (NULL);
		if (left > philos[i].time_to_die)
			ft_died(data, &philos[i]);
		i++;
	}
	return (NULL);
}
