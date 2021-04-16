/* Dica para compilação: clear && gcc -o run main.c -lpthread && ./run */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
// #include <semaphore.h>
#include <fcntl.h>

#define QUANTIDADE_FILOSOFOS 5
#define true 1
#define false 0

char nomeFilosofos[5][20] = {"Sócrates", "Platão", "Aristóteles", "Nietzsche", "Rousseau"};

void *filosofoThread(void *data);

pthread_mutex_t mutexes[QUANTIDADE_FILOSOFOS] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER};

// sem_t s;

int main(void)
{
    srand(time(NULL));
    pthread_t tids[QUANTIDADE_FILOSOFOS];
    int i = 0;

    // sem_init(&s, 0, 1); 

    for (i = 0; i < QUANTIDADE_FILOSOFOS; i++)
    {
        int *j = malloc(sizeof(int));
        *j = i;
        printf("Filósofo %s acabou de nascer.\n", nomeFilosofos[i]);
        pthread_create(&tids[i], NULL, filosofoThread, (void *)j);
    }

    for (i = 0; i < QUANTIDADE_FILOSOFOS; i++)
    {
        pthread_join(tids[i], NULL);
        printf("Thread id %ld returned\n", tids[i]);
    }

    return (1);
}

unsigned int geraTempo()
{
    unsigned int tempoAcao = rand() % 5;

    return tempoAcao;
}

void pensa(int id)
{
    unsigned int tempoAcao = geraTempo();
    printf("%s está pensando por %ds.\n", nomeFilosofos[id], tempoAcao);
    sleep(tempoAcao);
}

void ficaComFome(int id)
{
    printf("%s está com fome.\n", nomeFilosofos[id]);
}

void come(int id)
{
    unsigned int tempoAcao = geraTempo();
    printf("%s está comendo por %ds.\n", nomeFilosofos[id], tempoAcao);
    sleep(tempoAcao);
}

void *filosofoThread(void *data)
{
    int id = *((int *)data);
    free((int *)data);

    while (true)
    {
        // sem_wait(&s);

        pensa(id);
        ficaComFome(id);

        int esquerda = id;
        int direita = (id + 1) % QUANTIDADE_FILOSOFOS;

        while (true)
        {
            if (pthread_mutex_trylock(&(mutexes[esquerda])) != 0)
                continue;

            if (pthread_mutex_trylock(&(mutexes[direita])) != 0)
            {
                pthread_mutex_unlock(&(mutexes[esquerda]));
                continue;
            }

            printf("%s conseguiu os dois garfos.\n", nomeFilosofos[id]);
            break;
        }

        come(id);

        pthread_mutex_unlock(&(mutexes[esquerda]));
        pthread_mutex_unlock(&(mutexes[direita]));
        // sem_post(&s);
    }

    pthread_exit(NULL);
}
