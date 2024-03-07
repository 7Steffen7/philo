/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 20:57:23 by sparth            #+#    #+#             */
/*   Updated: 2024/03/07 16:16:11 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool check_input(int argc, char *argv[])
{
	int i;
	int j;	
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j]) {
			if (ft_isdigit(argv[i][j]) ||
				(argv[i][j] == '+' && ft_isdigit(argv[i][j + 1])))
			j++;
			else
				return (false);
		}
		i++;
	}
	return (true);
}
int create_philos(t_input *data)
{
	int i;
	struct timeval tp;
	
	i = 0;
	data->philo = (t_philo *)malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->philo)
		return (0);
	gettimeofday(&tp, NULL);
	while (i < data->philo_nbr)
	{
		data->philo[i].ID = i + 1;
		data->philo[i].philo_nbr = data->philo_nbr;
		data->philo[i].left_fork = i;
		data->philo[i].right_fork = (i + 1) % data->philo_nbr;
		data->philo[i].time2die = data->time2die;
		data->philo[i].time2eat = data->time2eat;
		data->philo[i].time2sleep = data->time2sleep;
		data->philo[i].meals2eat = data->meals2eat;
		data->philo[i].forks = data->forks;
		data->philo[i].init_time = tp.tv_usec / 1000 + tp.tv_sec * 1000;
		data->philo[i].death_count = data->philo[i].init_time;
		data->philo[i].data = data;
		i++;
	}
	return (1);
}

void init_struct(int argc, char *argv[], t_input *data)
{
	pthread_mutex_init(&data->death_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->check1, NULL);
	data->thread = NULL;
	data->forks = NULL;
	data->philo = NULL;
	data->philo_nbr = ft_atoi(argv[1]);
	data->time2die = ft_atoi(argv[2]);
	data->time2eat = ft_atoi(argv[3]);
	data->time2sleep = ft_atoi(argv[4]);
	data->is_dead = false;
	data->meal_count = 0;
	data->full = false;
	if (argc == 6)
		data->meals2eat = ft_atoi(argv[5]);
	else
		data->meals2eat = -1;
}

void *death_check(void *arg)
{
	t_input *data = (t_input *)arg;
	struct timeval tp;
	long current_time;
	int i;
	
	while (1)
	{
		i = 0;
		gettimeofday(&tp, NULL);
		current_time = tp.tv_usec / 1000 + tp.tv_sec * 1000;
		while (i < data->philo_nbr)
		{
			pthread_mutex_lock(&data->check1);
			if (data->philo[i].death_reset)
			{
				data->philo[i].death_count = current_time;
				data->philo[i].death_reset = 0;
			}
			pthread_mutex_unlock(&data->check1);
			if (current_time - data->philo[i].death_count == data->time2die)
			{
				pthread_mutex_lock(&data->death_lock);
				data->is_dead = 1;
				pthread_mutex_unlock(&data->death_lock);
				printf("%ld %d died\n", current_time - data->philo[i].init_time, data->philo[i].ID);
				return (NULL);
			}
			if (data->meal_count == data->meals2eat * data->philo_nbr)
			{
				data->full = true;
				return (NULL);
			}
			i++;
		}
	}
return (NULL);
}

void time2(int time, t_philo *data)
{
	long temp;
	struct timeval tp;
	gettimeofday(&tp, NULL);
	temp = tp.tv_usec / 1000 + tp.tv_sec * 1000;
	// if (data->philo_nbr % 2 != 0 && data->ID == data->philo_nbr &&
	// data->timestamp < 2) 	time *= 2;
	while (1)
	{
		gettimeofday(&tp, NULL);
		if (((tp.tv_usec / 1000 + tp.tv_sec * 1000) - temp) == time)
			return;
		pthread_mutex_lock(&data->data->death_lock);
		if (data->data->is_dead)
		{
			pthread_mutex_unlock(&data->data->death_lock);
			return;
		}
		pthread_mutex_unlock(&data->data->death_lock);
		usleep(50);
	}
	printf("error???\n");
	return;
}

bool print_func(char *status, t_philo *data)
{
	struct timeval tp;

	gettimeofday(&tp, NULL);
	data->timestamp = (tp.tv_usec / 1000 + tp.tv_sec * 1000) - data->init_time;
	pthread_mutex_lock(&data->data->death_lock);
	if (!data->data->is_dead && !data->data->full)
	{
		printf("%ld %d %s\n", data->timestamp, data->ID, status);
		pthread_mutex_unlock(&data->data->death_lock);
		return (true);
	}
	else
	{
		pthread_mutex_unlock(&data->data->death_lock);
		return (false);
	}
}

void *routine(void *arg)
{

	t_philo *data = (t_philo *)arg;
	if (data->ID % 2 == 0 || (data->ID % 2 != 0 && data->philo_nbr == data->ID && data->philo_nbr % 2 != 0 && data->ID != 1))
	{
		time2(data->time2eat, data);
		if (data->data->is_dead)
			return (NULL);
	}
	while (data->meals2eat--)
	{
		pthread_mutex_lock(&data->forks[data->left_fork]);
		print_func("has taken a fork", data);
		if (data->philo_nbr == 1)
		{
			pthread_mutex_unlock(&data->forks[data->left_fork]);
			return (NULL);
		}
		pthread_mutex_lock(&data->forks[data->right_fork]);
		print_func("has taken a fork", data);
		if (!print_func("is eating", data))
		{
			pthread_mutex_unlock(&data->forks[data->left_fork]);
			pthread_mutex_unlock(&data->forks[data->right_fork]);
			return (NULL);
		}
		pthread_mutex_lock(&data->data->check1);
		data->death_reset = 1;
		pthread_mutex_unlock(&data->data->check1);
		time2(data->time2eat, data);
		pthread_mutex_unlock(&data->forks[data->left_fork]);
		pthread_mutex_unlock(&data->forks[data->right_fork]);
		pthread_mutex_lock(&data->data->meal_lock);
		data->data->meal_count++;
		pthread_mutex_unlock(&data->data->meal_lock);
		if (!print_func("is sleeping", data))
			return (NULL);
		time2(data->time2sleep, data);
		if (!print_func("is thinking", data))
			return (NULL);
	}
	return (NULL);
}

int	free_return(t_input *data, bool return_value, int mutex_destroy)
{
	if (data->thread)
		free(data->thread);
	if (data->forks)
		free(data->forks);
	if (data->philo)
		free(data->philo);
	if (data)
		free(data);
	while (mutex_destroy--)
		pthread_mutex_destroy(&data->forks[mutex_destroy]);
	return (return_value);
}

bool preperation(int argc, char *argv[], t_input *data)
{
	int	i;

	i = 0;
	if (argc < 5 || argc > 6)
		return (0);
	if (!check_input(argc, argv))
		return (0);
	init_struct(argc, argv, data);
	data->thread = (pthread_t *)malloc(sizeof(pthread_t) * (data->philo_nbr + 1));
	if (!data->thread)
		return (free_return(data, 0, 0));
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->philo_nbr);
	if (!data->forks)
		return (free_return(data, 0, 0));
	while (i < data->philo_nbr)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (free_return(data, 1, i));
		i++;
	}
	if (!create_philos(data))
		return (free_return(data, 1, data->philo_nbr));
	return (1);
}

bool	threading(t_input *data)
{
	int	i;
	
	i = 0;
	while (i < data->philo_nbr)
	{
		if (pthread_create(&data->thread[i], NULL, &routine, &data->philo[i]) != 0)
			return (free_return(data, 0, data->philo_nbr));
		i++;
	}
	if (pthread_create(&data->thread[i], NULL, &death_check, data) != 0)
		return (free_return(data, 0, data->philo_nbr));
	i = 0;
	while (i < data->philo_nbr)
	{
		if (pthread_join(data->thread[i], NULL) != 0)
			return (free_return(data, 0, data->philo_nbr));
		i++;
	}
	if (pthread_join(data->thread[i], NULL) != 0)
		return (free_return(data, 0, data->philo_nbr));
	return (1);
}

int main(int argc, char *argv[])
{
	t_input *data;
	int i;

	data = (t_input *)malloc(sizeof(t_input));
	if (!data)
		return (free_return(data, 1, 0));
	if (!preperation(argc, argv, data))
		return (1);
	if (!threading(data))
		return (1);
	i = 0;
	while (i < data->philo_nbr)
	{
		if (pthread_mutex_destroy(&data->forks[i]) !=0)
		{
			printf("pthread_mutex_destroy failed");
			return (free_return(data, 1, 0));
		}
		i++;
	}
	return (free_return(data, 0, 0));
}
