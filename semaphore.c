#include <pthread.h>

struct SEM
{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
};
typedef struct SEM SEM;

SEM *semCreate(int initVal)
{
    SEM *sem = malloc(sizeof(SEM));
    if (!sem)
    {
        // error
    }

    sem->value = initVal;

    if (pthread_mutex_init(&(sem->mutex), NULL))
    {
        free(sem);
        // error
    }

    if (pthread_cond_init(&(sem->condition), NULL))
    {
        pthread_mutex_destroy(&(sem->mutex));
        free(sem);
        // error
    }

    return sem;
}

void semDestroy(SEM *sem)
{
    if (sem)
    {

        if (pthread_mutex_destroy(&(sem->mutex)))
        {
            // error
        }
        if (pthread_cond_destroy(&(sem->condition)))
        {
            // error
        }
        free(sem);
    }
}

void P(SEM *sem)
{
    pthread_mutex_lock(&(sem->mutex));
    while (sem->value < 1)
    {
        pthread_cond_wait(&(sem->condition), &(sem->mutex));
    }
    sem->value = sem->value - 1;
    pthread_mutex_unlock(&(sem->mutex));
}

void V(SEM *sem)
{
    pthread_mutex_lock(&(sem->mutex));
    sem->value = sem->value + 1;
    pthread_cond_signal(&(sem->condition));
    pthread_mutex_unlock(&(sem->mutex));
}