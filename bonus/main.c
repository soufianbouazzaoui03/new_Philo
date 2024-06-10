/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 10:38:39 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/10 22:14:31 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*routine(void *input)
{
	t_philo *philo;
	t_data *data;

	philo = (t_philo*)input;
	data = philo->data;
	while (1)
	{
		semaphore_wait(data->meal_lock);
		if(get_time() - philo->last_meal_time > philo->time_to_die)
		{
			semaphore_post(data->meal_lock);
			semaphore_wait(data->print_lock);
			printf("%lu %d died\n", (get_time() - philo->sim_start), philo->id);
			exit(1);
		}
		semaphore_post(data->meal_lock);
	}
	return (NULL);
}

void eat_sleep_think(t_philo *philo)
{
    int count;
    t_data *data;

    count = 1;
    data = philo->data;
    if (philo->meals == -1)
        count = 0;
    semaphore_wait(data->print_lock);
    printf("%zu %d is eating\n", get_time() - philo->sim_start, philo->id);
    semaphore_post(data->print_lock);
    ft_usleep(philo->time_to_eat);
    philo->meals_eaten += count;
    if (philo->meals != -1 && philo->meals_eaten >= philo->meals)
    {
        semaphore_post(data->forks);
        semaphore_post(data->forks);
        exit(2);
    }
    semaphore_wait(data->meal_lock);
    philo->last_meal_time = get_time();
    semaphore_post(data->meal_lock);
    semaphore_post(data->forks);
    semaphore_post(data->forks);
	semaphore_wait(data->print_lock);
	printf("%zu %d is sleeping\n", get_time() - philo->sim_start, philo->id);
	semaphore_post(data->print_lock);
    ft_usleep(philo->time_to_sleep);
    semaphore_wait(data->print_lock);
    printf("%zu %d is thinking\n", get_time() - philo->sim_start, philo->id);
    semaphore_post(data->print_lock);
}

void philo_sim(t_philo *philo, t_data *data)
{
    if (philo->id % 2 == 0)
        ft_usleep(philo->time_to_eat / 2); // Stagger the start time to avoid deadlock

    while (1)
    {
        semaphore_wait(data->forks);

        semaphore_wait(data->print_lock);
        printf("%zu %d has taken a fork\n", get_time() - philo->sim_start, philo->id);
        semaphore_post(data->print_lock);

        semaphore_wait(data->forks);

        semaphore_wait(data->print_lock);
        printf("%zu %d has taken a fork\n", get_time() - philo->sim_start, philo->id);
        semaphore_post(data->print_lock);

        eat_sleep_think(philo);
    }
}

void	kill_philos(t_data *data)
{
	int	i;

	i = 0;
    unlink_sim();
	while (i < data->philos_num)
	{
	    kill(data->philos[i].pid, SIGINT);
		i++;
	}
    exit(0);
}


int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		if(check_args(argc, argv))
			return (1);
		init_data(argc, argv, &data);
        unlink_sim();
	}
	else
		return (1);
	return (0);
}