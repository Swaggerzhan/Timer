//
// Created by Swagger on 2021/10/29.
//
#include "Clock.h"
#include <sys/time.h>


int Clock::kMillSecond2NanoSecond_ = 1000;
int Clock::kSecond2MillSecond_ = 1000 * 1000;
int Clock::kSecond2NanoSecond_ = 1000 * 1000 * 1000;


time_t Clock::now_ns() {
  timeval v{};
  gettimeofday(&v, nullptr);
  return v.tv_sec * kSecond2MillSecond_ + v.tv_usec;
}

time_t Clock::now_us() {
  timeval v{};
  gettimeofday(&v, nullptr);
  return v.tv_sec * kSecond2NanoSecond_ +
     v.tv_usec * kMillSecond2NanoSecond_;
}

time_t Clock::max_time() {
  time_t ret = 0;
  return ~ret;
}


