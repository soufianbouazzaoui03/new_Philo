/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 10:38:51 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/10 19:55:33 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>


typedef struct s_philo
{
	pthread_t		thread_id;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	pid_t				pid;
	int					id;
	size_t			last_meal_time;
	size_t			sim_start;
	int				meals;
	int				philos_num;
	int				meals_eaten;
	struct s_data			*data;
	
}	t_philo;

typedef struct s_data
{
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				philos_num;
	int				meals_num;
	t_philo 		philos[201];
	sem_t			*forks;
	sem_t			*print_lock;
	sem_t			*alive_lock;
	sem_t			*meal_lock;
}	t_data;

size_t	get_time(void);
void	ft_usleep(size_t time_ms);
void semaphore_post(sem_t *sem);
void semaphore_wait(sem_t *sem);
int	ft_atoi(const char *str);
int	check_args(int argc, char **argv);
void	init_data(int argc, char **argv, t_data *data);
void	*routine(void *input);
void eat_sleep_think(t_philo *philo);
void philo_sim(t_philo *philo, t_data *data);
void	kill_philos(t_data *data);
void    unlink_sim(void);

#endif