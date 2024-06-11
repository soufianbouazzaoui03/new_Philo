/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:06:29 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/11 18:42:26 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	semaphore_wait(sem_t *sem)
{
	int	n;

	n = sem_wait(sem);
	if (n != 0)
		exit_close();
}

void	semaphore_post(sem_t *sem)
{
	int	n;

	n = sem_post(sem);
	if (n != 0)
		exit_close();
}

void	unlink_sim(void)
{
	sem_unlink("forks");
	sem_unlink("print_lock");
	sem_unlink("alive_lock");
	sem_unlink("meal_lock");
}