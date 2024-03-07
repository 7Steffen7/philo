/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 12:41:04 by sparth            #+#    #+#             */
/*   Updated: 2024/03/07 00:08:34 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_input t_input;

typedef struct s_philo
{
	long timestamp;
	int ID;
	int philo_nbr;
	int left_fork;
	int right_fork;
	int time2die;
	int time2eat;
	int time2sleep;
	int meals2eat;
	bool death_reset;
	long init_time;
	long death_count;
	pthread_mutex_t *forks;
	t_input *data;
}	t_philo;

typedef struct s_input {
	int philo_nbr;
	int time2die;
	int time2eat;
	int time2sleep;
	int meals2eat;
	int meal_count;
	bool is_dead;
	bool full;
	t_philo *philo;
	pthread_t *thread;
	pthread_mutex_t *forks;
	pthread_mutex_t death_lock;
	pthread_mutex_t meal_lock;
	pthread_mutex_t check1;
}	t_input;

int ft_atoi(const char *str);
int ft_isdigit(int c);
