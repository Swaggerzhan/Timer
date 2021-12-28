//
// Created by Swagger on 2021/10/29.
//
#include <algorithm>
#include <thread>
#include <utility>
#include <mutex>
#include "TimerThread.h"
#include "futex.h"

#ifdef DEBUG
//for debug
#include <iostream>

using std::cout;
using std::endl;
#endif


bool timerGreater(const Timer* left, const Timer* right) {
  return left > right;
}


class TimerThread::Bucket {
public:
  Bucket();
  ~Bucket();

  // call by user
  bool push(Timer* t);
  // call by TimerThread
  Timer* take();


private:

  std::mutex mutex_;
  std::atomic<time_t> earliest_expired_time_;
  Timer* head_;

};

TimerThread::Bucket::Bucket()
: mutex_()
, head_(nullptr)
, earliest_expired_time_(Time::microStampMax())
{
}

TimerThread::Bucket::~Bucket() {
  Timer* cur = head_;
  while (cur) {
    Timer* next = cur->next;
    delete cur;
    cur = next;
  }
}


bool TimerThread::Bucket::push(Timer* t) {
  time_t expired = t->expired.microStamp();
  std::unique_lock<std::mutex> lockGuard(mutex_);
  if (head_ == nullptr) {
    head_ = t;
    earliest_expired_time_.store(expired, std::memory_order_release);
    return true; // earlier timer add at this bucket
  } else {
    t->next = head_;
    head_ = t;
    if (expired < earliest_expired_time_.load(std::memory_order_acquire)){
      earliest_expired_time_.store(expired, std::memory_order_release);
      return true; // earlier timer add at this bucket
    }
  }
  return false;
}


Timer* TimerThread::Bucket::take() {
  std::unique_lock<std::mutex> lockGuard(mutex_);
  if (!head_)
    return nullptr;
  Timer* ret = head_;
  head_ = nullptr;
  return ret;
}


TimerThread::TimerThread(int bucketCount)
: global_expired_time_(Time::microStampMax())
, bucketCount_(bucketCount)
, bucket_vec_()
, triggered_num_(0)
, running_(false)
, signal_(1)
{
  for (int i=0; i<bucketCount; i++) {
    bucket_vec_.push_back(new TimerThread::Bucket);
  }
  start();
}

TimerThread::~TimerThread() {
  stop();
  for (int i=0; i<bucketCount_; i++){
    delete bucket_vec_[i];
  }
}


bool TimerThread::start() {
  if (running_){
    return false;
  }
  running_ = true;
  std::thread t(std::bind(&TimerThread::timerThread, this));
  t.detach();
  return true;
}

void TimerThread::stop() {
  running_ = false;
}

Timer* TimerThread::setTimer(TimerThread::timeoutCB cb, Time timeout) {
  int bucket_num = pthread_self() % bucketCount_; // TODO: can do faster?
  Timer* timer = new Timer(std::move(cb), timeout);
  if (bucket_vec_[bucket_num]->push(timer)) {
    if (timer->expired.microStamp() < global_expired_time_.load()) {
      futex_wake(&signal_, 1);
    }
  }
  return timer;
}


void TimerThread::timerThread() {
  std::vector<Timer*> timer_heap;
  timer_heap.reserve(4096);

  while (running_) {

    adjust_bucket:
    global_expired_time_.store(Time::microStampMax(), std::memory_order_release);
    for (int i=0; i<bucketCount_; i++) {
      for (Timer* timer = bucket_vec_[i]->take(); ;) {
        Timer* next;
        if (!timer)
          break;
        next = timer->next;
        if (timer->isDelete){
          delete timer;
          timer = next;
          continue;
        }
        timer_heap.push_back(timer);
        std::push_heap(timer_heap.begin(), timer_heap.end(), timerGreater);
        timer = next;
      }
    }
    // handle timeout Timer
    while (!timer_heap.empty()) {
      Timer* timer = timer_heap[0];
      if ( Time().microStamp() < timer->expired.microStamp() ){
        // not Timeout Yet.
        break;
      }
      // before we run timeout cb,
      // check if there has earlier timer in bucket!
      if ( global_expired_time_.load() < timer->expired.microStamp() ){
        goto adjust_bucket;
      }
      std::pop_heap(timer_heap.begin(), timer_heap.end(), timerGreater);
      timer_heap.pop_back();
      if (timer->runTimeout()){
        triggered_num_ ++;
      }
      delete timer;
    }
    // TODO: sleep
    time_t nextWakeTime = Time::microStampMax();
    if (!timer_heap.empty()) {
      nextWakeTime = timer_heap[0]->expired.microStamp();
    }
    if (global_expired_time_.load() < nextWakeTime){
      goto adjust_bucket;
    }
    futex_wait(&signal_, 1, nextWakeTime);
  }
}
