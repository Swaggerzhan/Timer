//
// Created by Swagger on 2021/10/29.
//

#ifndef TIMER_CLOCK_H
#define TIMER_CLOCK_H

typedef unsigned long time_t;

class Clock{
public:
  // get us
  static time_t now_us();
  // get ns
  static time_t now_ns();

  static time_t max_time();

private:
  static int kSecond2MillSecond_;
  static int kSecond2NanoSecond_;
  static int kMillSecond2NanoSecond_;
};

#endif //TIMER_CLOCK_H
