#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>

class Mutex
{
private:
    pthread_mutex_t mutex;
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    bool trylock();
};

#endif
