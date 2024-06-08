/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 09:52:32 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/04 09:59:29 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t		thread_id;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				l_fork;
	int				r_fork;
	int				id;
	size_t			last_meal_time;
	size_t			sim_start;
	int				meals;
	int				philos_num;
	int				meals_eaten;
	bool			stop;
	struct s_data	*data;
	pthread_mutex_t	*write;
	pthread_mutex_t	*read;
}	t_philo;

typedef struct s_monitro
{
	pthread_t		thread_id;
	struct s_data	*data;
}	t_monitor;

typedef struct s_data
{
	int				err[3];
	int				lock_error;
	int				forks_err[201];
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				philos_num;
	int				meals_num;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write;
	pthread_mutex_t	read;
	pthread_mutex_t	ready;
	bool			death;
	bool			all_philos_ready;
	t_monitor		*monitor;
}	t_data;

int		ft_atoi(const char *str);
void	ft_usleep(size_t time_ms);
void	asign_forks(t_philo *philo, t_data *data, int pos);
size_t	get_time(void);
void	*monitoring(void *input);
int		check_args(int argc, char **argv);
void	destroy_mtx(t_data *data);
int		check_meals(t_philo *philos, int num, int meals, t_data *data);
void	ft_died(t_data *data, t_philo *philo);
void	*monitoring(void *input);
void	creat_mtr_join(t_data *data, t_monitor *monitor, t_philo *philo);
void	creat_philos(t_data *data, t_monitor *monitor, size_t start);
int		data_alloc(t_data *data, int argc, char **argv);
void	init_data(t_data *data, int argc, char **argv);
void	*philo_routine(void *input);

#endif