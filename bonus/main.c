/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 10:38:39 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/10 15:35:18 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
			//semaphore_post(data->print_lock);
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
	int i;

	i = 0;
	while(i < data->philos_num)
	{
		if(kill(data->philos[i].pid, 2) > 0)
			perror("kill");
		i++;
	}
	exit(0);
}

void	creat_philos(t_data *data, size_t start)
{
	int i;
	t_philo *philos;
	int exit_status;
	int	meals;

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
		{
			perror("fork");
			exit(1);
		}
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
        int pid = waitpid(-1, &exit_status, 0);
        if (pid == -1)
        {
            perror("waitpid");
            break;
        }
        if (WIFEXITED(exit_status))
        {
            exit_status = WEXITSTATUS(exit_status);
            if (exit_status == 2)
                meals++;
            if (exit_status == 1 || meals == data->philos_num)
            {
                kill_philos(data);
                break;
            }
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
	if(data->forks == SEM_FAILED)
		exit(1);
	data->print_lock = sem_open("print_lock", O_CREAT | O_EXCL, 0644, 1);
	if(data->print_lock == SEM_FAILED)
		exit(1);
	data->alive_lock = sem_open("alive_lock", O_CREAT | O_EXCL, 0644, 1);
	if(data->alive_lock == SEM_FAILED)
		exit(1);
	data->meal_lock = sem_open("meal_lock", O_CREAT | O_EXCL, 0644, 1);
	if(data->meal_lock == SEM_FAILED)
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

int	main(int argc, char **argv)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	// if(check_args(argc, argv))
	// 	return (1);
	init_data(argc, argv, data);
}