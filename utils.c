/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:17:18 by sparth            #+#    #+#             */
/*   Updated: 2024/02/26 13:18:51 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	size_t	i;
	size_t	count;
	size_t	minuscheck;

	i = 0;
	count = 0;
	minuscheck = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minuscheck *= -1;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9'))
	{
		count *= 10;
		count += str[i] - 48;
		i++;
	}
	return (count * minuscheck);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}