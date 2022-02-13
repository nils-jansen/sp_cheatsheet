#include <stdlib.h>
#include <limits.h>
#include <stdatomic.h>
#include <stdbool.h>

struct BNDBUF
{
    int size;
    int *buf;
    SEM *put;
    SEM *get;
    atomic_int nextPut;
    atomic_int nextGet;
};
struct SEM
{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
};
typedef struct SEM SEM;
typedef struct BNDBUF BNDBUF;

BNDBUF *bbCreate(size_t size)
{
    if (size > INT_MAX)
    {
        return NULL;
    }

    BNDBUF *boundedBuffer = malloc(sizeof(BNDBUF));
    if (!boundedBuffer)
    {
        return NULL;
    }
    SEM *putSemaphore = semCreate((int)size);
    if (!putSemaphore)
    {
        free(boundedBuffer);
        return NULL;
    }
    SEM *getSemaphore = semCreate(0);
    if (!getSemaphore)
    {
        free(boundedBuffer);
        semDestroy(putSemaphore);
        return NULL;
    }

    int *buffer = calloc(size, sizeof(int));
    if (!buffer)
    {
        free(boundedBuffer);
        semDestroy(putSemaphore);
        semDestroy(getSemaphore);
        return NULL;
    }

    boundedBuffer->size = (int)size;
    boundedBuffer->put = putSemaphore;
    boundedBuffer->get = getSemaphore;
    boundedBuffer->buf = buffer;
    boundedBuffer->nextGet = ATOMIC_VAR_INIT(0);
    boundedBuffer->nextPut = ATOMIC_VAR_INIT(0);

    return boundedBuffer;
}

void bbDestroy(BNDBUF *bb)
{
    if (!bb)
    {
        return;
    }

    free(bb->buf);
    semDestroy(bb->put);
    semDestroy(bb->get);
    free(bb);
}

void bbPut(BNDBUF *bb, int value)
{
    P(bb->put);
    int writeIndex = bb->nextPut;
    bb->buf[writeIndex] = value;
    bb->nextPut = (writeIndex + 1) % bb->size;
    V(bb->get);
}

int bbGet(BNDBUF *bb)
{
    int valueFromBuffer, current, next;
    P(bb->get);
    do
    {
        current = bb->nextGet;
        next = (current + 1) % bb->size;
        valueFromBuffer = bb->buf[current];
    } while (atomic_compare_exchange_strong(&(bb->nextGet), &current, next) == false);
    V(bb->put);
    return valueFromBuffer;
}