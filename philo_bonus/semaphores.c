/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:06:29 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/12 16:48:12 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	semaphore_wait(sem_t *sem, t_data *data)
{
	int	n;

	n = sem_wait(sem);
	if (n != 0)
		exit_close(data);
}

void	semaphore_post(sem_t *sem, t_data *data)
{
	int	n;

	n = sem_post(sem);
	if (n != 0)
		exit_close(data);
}

void	unlink_sim(t_data *data)
{
	sem_close(data->forks);
	sem_close(data->print_lock);
	sem_close(data->meal_lock);
	sem_close(data->alive_lock);
	sem_unlink("forks");
	sem_unlink("print_lock");
	sem_unlink("alive_lock");
	sem_unlink("meal_lock");
}
