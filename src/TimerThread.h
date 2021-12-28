//
// Created by Swagger on 2021/10/29.
//

#ifndef TIMER_TIMERTHREAD_H
#define TIMER_TIMERTHREAD_H

#include <vector>
#include <functional>
#include <atomic>
#include "Time.h"
#include "Timer.h"


class Timer;

class TimerThread{
public:
  typedef std::function<void()> timeoutCB;
  class Bucket;
  TimerThread(int bucketCount=4);
  ~TimerThread();

  bool start();
  Timer* setTimer(timeoutCB cb, Time timeout);
  void stop();


private:

  void timerThread();

private:
  pthread_t timer_thread_id_;
  std::atomic<time_t> global_expired_time_;
  std::atomic<bool> running_;
  int bucketCount_;
  std::vector<Bucket*> bucket_vec_;
  int signal_;
  int triggered_num_;
};

#endif //TIMER_TIMERTHREAD_H
