//
// Created by Swagger on 2021/10/29.
//

#ifndef TIMER_TIMERTHREAD_H
#define TIMER_TIMERTHREAD_H

#include <vector>
#include <functional>
#include <pthread.h>
#include <mutex>
#include <atomic>

typedef std::function<void()> timeoutCB;

class Bucket;
class Timer;

class TimerThread{
public:
  friend Bucket;
  TimerThread();
  ~TimerThread();

  bool start();

  time_t getGlobalExpiredTime();

private:
  static void* thread_entry(void*);
  void run(); // entry

private:
  pthread_t timer_thread_id_;
  std::atomic<time_t> global_expired_time_;
  int bucket_num_;
  std::vector<Bucket*> bucket_vec_;
  std::mutex mutex_;
  int signal_;
  int triggered_num_;
};




#endif //TIMER_TIMERTHREAD_H
