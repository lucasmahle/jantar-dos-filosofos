/* Dica para compilação: gcc -o run main.c -lpthread && ./run */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define QUANTIDADE_FILOSOFOS 5
#define true 1
#define false 0

char nomeFilosofos[5][20] = {"Sócrates", "Platão", "Aristóteles", "Nietzsche", "Rousseau"};

void *filosofoThread(void *data);

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t s;

int x = 0;

int main(void)
{
    srand(time(NULL));
    pthread_t tids[QUANTIDADE_FILOSOFOS];
    int i = 0;

    sem_init(&s, 0, 1); // inicializa o semáforo com valor 1

    for (i = 0; i < QUANTIDADE_FILOSOFOS; i++)
    {
        int *j = malloc(sizeof(int));
        *j = i;
        pthread_create(&tids[i], NULL, filosofoThread, (void *)j);
    }

    for (i = 0; i < QUANTIDADE_FILOSOFOS; i++)
    {
        pthread_join(tids[i], NULL);
        printf("Thread id %ld returned\n", tids[i]);
    }

    return (1);
}

void pensa()
{
    // printf("%d", rand() % 5000);
    // sleep(rand() % 5000);
}

void come()
{
}

void *filosofoThread(void *data)
{
    int id = *((int *)data);
    free((int *)data);

    // demonstração tratamento exclusão mútua utilizando semáforo
    while (true)
    {
        sem_wait(&s);

        x++;
        pensa();
        printf("%s tem x igual a %d.\n", nomeFilosofos[id], x);

        sem_post(&s);
    }

    pthread_exit(NULL);
}
