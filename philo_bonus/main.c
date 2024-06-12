/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 10:38:39 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/12 16:50:05 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*routine(void *input)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)input;
	data = philo->data;
	while (1)
	{
		semaphore_wait(data->meal_lock, data);
		if (get_time() - philo->last_meal_time > philo->time_to_die)
		{
			semaphore_post(data->meal_lock, data);
			semaphore_wait(data->print_lock, data);
			printf("%lu %d died\n", (get_time() - philo->sim_start), philo->id);
			exit(1);
		}
		semaphore_post(data->meal_lock, data);
	}
	return (NULL);
}

void	sleep_think(t_philo *philo, t_data *data)
{
	semaphore_wait(data->meal_lock, data);
	philo->last_meal_time = get_time();
	semaphore_post(data->meal_lock, data);
	semaphore_post(data->forks, data);
	semaphore_post(data->forks, data);
	semaphore_wait(data->print_lock, data);
	printf("%zu %d is sleeping\n", get_time() - philo->sim_start, philo->id);
	semaphore_post(data->print_lock, data);
	ft_usleep(philo->time_to_sleep);
	semaphore_wait(data->print_lock, data);
	printf("%zu %d is thinking\n", get_time() - philo->sim_start, philo->id);
	semaphore_post(data->print_lock, data);
}

void	eat_sleep_think(t_philo *philo)
{
	int		count;
	t_data	*data;

	count = 1;
	data = philo->data;
	if (philo->meals == -1)
		count = 0;
	semaphore_wait(data->print_lock, data);
	printf("%zu %d is eating\n", get_time() - philo->sim_start, philo->id);
	semaphore_post(data->print_lock, data);
	ft_usleep(philo->time_to_eat);
	philo->meals_eaten += count;
	if (philo->meals != -1 && philo->meals_eaten >= philo->meals)
	{
		semaphore_post(data->forks, data);
		semaphore_post(data->forks, data);
		exit(2);
	}
	sleep_think(philo, data);
}

void	philo_sim(t_philo *philo, t_data *data)
{
	if (philo->id % 2 == 0)
		ft_usleep(philo->time_to_eat / 2);
	while (1)
	{
		semaphore_wait(data->forks, data);
		semaphore_wait(data->print_lock, data);
		printf("%zu %d has taken a fork\n",
			get_time() - philo->sim_start, philo->id);
		semaphore_post(data->print_lock, data);
		semaphore_wait(data->forks, data);
		semaphore_wait(data->print_lock, data);
		printf("%zu %d has taken a fork\n",
			get_time() - philo->sim_start, philo->id);
		semaphore_post(data->print_lock, data);
		eat_sleep_think(philo);
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		if (check_args(argc, argv))
			return (1);
		init_data(argc, argv, &data);
		unlink_sim(&data);
	}
	else
		return (1);
	return (0);
}
