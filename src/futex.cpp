//
// Created by Swagger on 2021/12/27.
//

#include "futex.h"
#include <unistd.h>

#include <iostream>
using std::endl;
using std::cout;

#ifdef __linux__
#include <syscall.h>
#include <linux/futex.h>

long futex_wait(void* addr, int expected, time_t time){
  timespec timeout{};
  timespec* ptimeout;
  if (time == Time::microStampMax()) {
    ptimeout = nullptr;
  }else {
    time_t interval = (time * Time::kMicroSecond2NanoSecond) - Time().nanoStamp();
    timeout.tv_sec = interval / Time::kSecond2NanoSecond;
    timeout.tv_nsec = interval % Time::kSecond2NanoSecond;
    ptimeout = &timeout;
  }
  return syscall(SYS_futex, addr,
                 (FUTEX_WAIT | FUTEX_PRIVATE_FLAG),
                 expected, ptimeout, nullptr, 0);
}

long futex_wake(void* addr, int expected){
  return syscall(SYS_futex, addr,
                 (FUTEX_WAKE | FUTEX_PRIVATE_FLAG),
                 expected, nullptr, nullptr, 0);
}

#else
#error "Unsupported OS"
#endif

