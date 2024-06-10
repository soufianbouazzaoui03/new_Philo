/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:53:10 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/10 16:24:29 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	creat_philos(t_data *data, size_t start)
{
	int		i;
	t_philo	*philos;
	int		exit_status;
	int		meals;

	i = 0;
	meals = 0;
	philos = data->philos;
	while (i < data->philos_num)
	{
		philos = data->philos + i;
		philos->id = i + 1;
		philos->time_to_die = data->time_to_die;
		philos->time_to_eat = data->time_to_eat;
		philos->time_to_sleep = data->time_to_sleep;
		philos->meals = data->meals_num;
		philos->meals_eaten = 0;
		philos->data = data;
		philos->pid = fork();
		philos->sim_start = start;
		philos->last_meal_time = philos->sim_start;
		if(philos->pid == -1)
			exit(1);
		if (philos->pid == 0)
		{
			if(pthread_create(&philos->thread_id, NULL, routine, philos) != 0)
				exit(1);
			philo_sim(philos, data);
			pthread_join(philos->thread_id, NULL);
		}
		i++;
	}
	i = 0;
	while (1)
	{
		waitpid(-1, &exit_status, 0);
		if (WIFEXITED(exit_status))
		{
			exit_status = WEXITSTATUS(exit_status);
			if (exit_status == 2)
				meals++;
			if (exit_status == 1 || meals == data->philos_num)
				kill_philos(data);
		}
	}
}

void	init_semaphores(t_data *data)
{
	sem_unlink("forks");
	sem_unlink("print_lock");
	sem_unlink("alive_lock");
	sem_unlink("meal_lock");
	data->forks = sem_open("forks", O_CREAT | O_EXCL, 0644, data->philos_num);
	if (data->forks == SEM_FAILED)
		exit(1);
	data->print_lock = sem_open("print_lock", O_CREAT | O_EXCL, 0644, 1);
	if (data->print_lock == SEM_FAILED)
		exit(1);
	data->alive_lock = sem_open("alive_lock", O_CREAT | O_EXCL, 0644, 1);
	if (data->alive_lock == SEM_FAILED)
		exit(1);
	data->meal_lock = sem_open("meal_lock", O_CREAT | O_EXCL, 0644, 1);
	if (data->meal_lock == SEM_FAILED)
		exit(1);
}

void	init_data(int argc, char **argv, t_data *data)
{
	data->philos_num = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_num = ft_atoi(argv[5]);
	else if (argc == 5)
		data->meals_num = -1;
	init_semaphores(data);
	creat_philos(data, get_time());
}