//
// Created by Swagger on 2021/10/29.
//

#include "futex.h"
#include <linux/futex.h>
#include <unistd.h>
#include <syscall.h>

long futex_wait(int* addr, int expected, timespec* timeout){
  return syscall(SYS_futex, addr,
                 (FUTEX_WAIT | FUTEX_PRIVATE_FLAG),
                 expected, timeout, nullptr, 0);
}

long futex_wake(int* addr, int expected, timespec* timeout){
  return syscall(SYS_futex, addr,
                 (FUTEX_WAKE | FUTEX_PRIVATE_FLAG),
                 expected, timeout, nullptr, 0);
}




