//
// Created by Swagger on 2021/10/29.
//
#include "Bucket.h"
#include "TimerThread.h"
#include "Clock.h"
#include "futex.h"

Bucket::Bucket(TimerThread* timerThread)
: head_(nullptr)
, list_expired_time_(Clock::max_time())
, timer_thread_(timerThread)
{}

Bucket::~Bucket() {
  Timer* node = head_;
  while (node){
    Timer* next = node->next;
    delete node;
    node = next;
  }
  head_ = nullptr;
}

// call by worker thread
void Bucket::push(Timer *timer) {
  std::lock_guard<std::mutex> lockGuard(mutex_);
  timer->next = head_;
  head_ = timer;
  // check time
  if (timer->expired > list_expired_time_)
    return;

  list_expired_time_.store(timer->expired,
                           std::memory_order_relaxed);
  if (list_expired_time_ > timer_thread_->getGlobalExpiredTime())
    return;
  // wakeup timerThread
  futex_wake(&timer_thread_->signal_, 1);
}


Timer* Bucket::take(){
  std::lock_guard<std::mutex> lockGuard(mutex_);
  list_expired_time_.store(Clock::max_time(),
                           std::memory_order_relaxed);
  Timer* ret = head_;
  head_ = nullptr;
  return ret;
}





