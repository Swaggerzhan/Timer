//
// Created by Swagger on 2021/10/29.
//

#ifndef TIMER_FUTEX_H
#define TIMER_FUTEX_H

#include <sys/time.h>

long futex_wait(int* addr, int expected, timespec* timeout);

long futex_wake(int *addr, int expected);


#endif //TIMER_FUTEX_H
