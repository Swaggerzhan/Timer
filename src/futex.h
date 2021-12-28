//
// Created by Swagger on 2021/12/27.
//

#ifndef TIMER_FUTEX_H
#define TIMER_FUTEX_H

#include <sys/time.h>
#include "Time.h"

long futex_wait(void* addr, int expected, time_t timeout);

long futex_wake(void *addr, int expected);


#endif //TIMER_FUTEX_H
