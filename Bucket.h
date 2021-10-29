//
// Created by Swagger on 2021/10/29.
//

#ifndef TIMER_BUCKET_H
#define TIMER_BUCKET_H

#include <functional>
#include <mutex>
#include <atomic>

typedef std::function<void()> timeoutCB;

class TimerThread;
struct Timer{
  std::atomic<bool> deleted;
  time_t expired;
  timeoutCB cb;
  Timer* next;
  // TODO: for reuse timer, don't delete it!
  bool (*run_and_delete)();
};


class Bucket{
public:
  Bucket(TimerThread*);
  ~Bucket();

  // call by worker
  void push(Timer* timer);

  // you shouldn't call it,
  // this function is called by timerThtread
  Timer* take();

private:
  std::atomic<time_t> list_expired_time_;
  Timer* head_;
  std::mutex mutex_;
  TimerThread* timer_thread_;
};

#endif //TIMER_BUCKET_H
