#include "semaphore.h"

semaphore::semaphore(int value = 1) :count{ value }, wakeups{ 0 }
{
}


semaphore::~semaphore()
{
}

semaphore sendSemaphore(0);
semaphore recvSemaphore(0);
