//
// Created by swagger on 2021/12/28.
//
#include "Timer.h"

#include <iostream>
using std::endl;
using std::cout;

Timer::Timer(Timer::timeoutCB cb, Time timeout)
: expired(timeout)
, isDelete(false)
, cb(std::move(cb))
, next(nullptr)
{
}


void Timer::cancel() {
  isDelete.store(true, std::memory_order_seq_cst);
}


bool Timer::runTimeout() {
  if (!isDelete.load(std::memory_order_seq_cst)) {
    cb();
    return true;
  }
  return false;
}

void Timer::setExpired(Time t) {
  expired = t;
}

void Timer::addExpired(Time t) {
  expired += t;
}

bool Timer::operator < (Timer &right) {
  return expired < right.expired;
}

bool Timer::operator > (Timer &right) {
  return expired > right.expired;
}