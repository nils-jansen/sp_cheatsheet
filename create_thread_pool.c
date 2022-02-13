#include <pthread.h>

#define MY_SIZE 5

void action() {}

void create_thread_pool()
{
    for (int i = 0; i < MY_SIZE; i++)
    {
        pthread_t tid;
        if (pthread_create(tid, NULL, action, NULL))
        {
            // error
        }

        if (pthread_detach(tid))
        {
            // error
        }
    }
}