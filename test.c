#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex;

void *roll_dice(void *param)
{
	pthread_mutex_lock(&mutex);
	int value = (rand() % 6) + 1;
	int *result = malloc(sizeof(int));
	*result = value;
	printf("Thread res %d\n", *result);
	pthread_mutex_unlock(&mutex);
	return ((void *) result);
}

int	main()
{
	srand(time(NULL));
	pthread_t	th[8];
	int			i;
	int			*res;

	i = 0;
	pthread_mutex_init(&mutex, NULL);
	while (i < 8)
	{
		pthread_create(&th[i], NULL, &roll_dice, NULL);
		i++;
	}
	i = 0;
	while (i < 8)
	{
		pthread_join(th[i], (void **) &res);
		printf("Thread result %d\n", *res);
		i++;
	}
	// printf("Thread result %p\n", res);
	pthread_mutex_destroy(&mutex);
	return (0);
}