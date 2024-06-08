/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:06:29 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/05 15:11:48 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void semaphore_wait(sem_t *sem)
{
    int n = sem_wait(sem);
    if (n != 0) 
        exit(1);
}

void semaphore_post(sem_t *sem)
{
    int n = sem_post(sem);
    if (n != 0) 
        exit(1);
}