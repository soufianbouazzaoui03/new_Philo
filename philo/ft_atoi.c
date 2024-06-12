/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soel-bou <soel-bou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 03:50:54 by soel-bou          #+#    #+#             */
/*   Updated: 2024/06/12 19:59:14 by soel-bou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int		sign;
	long	res;
	int		i;

	(1) && (i = 0, sign = 1, res = 0);
	while (((str[i] >= 9 && str[i] <= 13) || (str[i] == 32)))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign *= -1;
	if (!str[i])
		return (-1);
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - 48) + (res * 10);
		if ((res * sign) > INT_MAX || (res * sign) < INT_MIN)
			return (-1);
		i++;
	}
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		return (-1);
	return (res * sign);
}

void	init_err(t_data *data)
{
	int	i;

	i = 0;
	data->err[0] = 1;
	data->err[1] = 1;
	data->err[2] = 1;
	while (i < data->philos_num)
		data->forks_err[i++] = 1;
}
