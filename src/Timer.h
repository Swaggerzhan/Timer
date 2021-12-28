//
// Created by swagger on 2021/12/28.
//

#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include "Time.h"
#include <functional>
#include <atomic>

class Bucket;
class TimerThread;

struct Timer {
public:
  typedef std::function<void()> timeoutCB;

  Timer(timeoutCB, Time time);
  ~Timer() = default;

  void cancel();


  // call by TimerThread
  bool runTimeout();


  // set
  void setExpired(Time);
  void addExpired(Time);

  bool operator < (Timer&);
  bool operator > (Timer&);


  Time expired;
  std::atomic<bool> isDelete;
  timeoutCB cb;
  Timer* next;

  // TODO: ticker....
  Time interval;

};



#endif //TIMER_TIMER_H
