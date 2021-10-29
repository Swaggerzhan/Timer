//
// Created by Swagger on 2021/10/29.
//
#include <algorithm>
#include "TimerThread.h"
#include "Clock.h"
#include "Bucket.h"

inline bool timerGreater(Timer* a, Timer* b){
  return a->expired > b->expired;
}

void* TimerThread::thread_entry(void* arg){
  TimerThread* t = static_cast<TimerThread*> (arg);
  t->run();
  return nullptr;
}

bool TimerThread::start() {

  for (int i=0; i<bucket_num_; i++){
    bucket_vec_.push_back(new Bucket(this));
  }
  global_expired_time_.store(Clock::max_time(),
                             std::memory_order_relaxed);

  // timer thread
  int ret = pthread_create(&timer_thread_id_, nullptr,
                    thread_entry, this);
  if (!ret)
    return true;
  return false;
}


time_t TimerThread::getGlobalExpiredTime() {
  return global_expired_time_.load(std::memory_order_relaxed);
}

void TimerThread::run() {

  std::vector<Timer*> timer_heap;
  timer_heap.reserve(4096);

  // timerThread main logic
  while (true){
    adjust_bucket:
    {
      std::lock_guard<std::mutex> lockGuard(mutex_);
      global_expired_time_.store(Clock::max_time(),
                                 std::memory_order_relaxed);
    }

    // take timer from all bucket,
    // and insert into heap
    for (int idx=0; idx<bucket_num_; idx++){
      for (Timer* timer = bucket_vec_[idx]->take(); ;){
        Timer* next;
        if (!timer)
          break;
        next = timer->next;
        if (timer->deleted.load(std::memory_order_relaxed)){
          delete timer;
          timer = next;
          continue;
        }
        timer_heap.push_back(timer);
        std::sort_heap(timer_heap.begin(), timer_heap.end(), timerGreater);
        timer = next;
      }
    }

    // try to handle timer_heap,
    // take out timer and run timeout callback
    while (!timer_heap.empty()){
      Timer* timer = timer_heap[0];
      // not timeout yet
      if (Clock::now_us() < timer->expired)
        break;
      // there has more earlier timer in bucket
      {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        if (timer->expired > global_expired_time_.load(std::memory_order_relaxed))
          goto adjust_bucket;
        // TODO: is this cause deadlock?
      }
      std::pop_heap(timer_heap.begin(), timer_heap.end(), timerGreater);
      timer_heap.pop_back();
      if (timer->run_and_delete()){
        triggered_num_ ++;
      }
    }
    time_t next_expired_time = Clock::max_time();
    if (!timer_heap.empty())
      next_expired_time = timer_heap[0]->expired;

    // TODO: do the last



  }


}



