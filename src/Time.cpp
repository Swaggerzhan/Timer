//
// Created by swagger on 2021/12/26.
//

#include "Time.h"
#include <iostream>
#include <limits>

using std::cout;
using std::endl;
using std::string;

const time_t Time::kSecond2NanoSecond = 1000 * 1000 * 1000; // s -> ns
const time_t Time::kSecond2MicroSecond = 1000 * 1000; // s -> us
const time_t Time::kMicroSecond2NanoSecond = 1000; // us -> ns


// for debug
void showDiff(Time* start, Time* end) {
  if (end->microStamp() < start->microStamp())
    cout << "end < start" << endl;
  time_t interval = end->microStamp() - start->microStamp();
  cout << interval / Time::kSecond2MicroSecond << " s";
  cout << interval % Time::kSecond2MicroSecond << " us" << endl;
}


Time::Time()
: tv_()
{
  gettimeofday(&tv_, nullptr);
}

Time::Time(timeval &t)
: tv_(t)
{}

Time::Time(timespec &t)
: tv_()
{
  tv_.tv_sec = t.tv_sec;
  tv_.tv_usec = t.tv_nsec / kMicroSecond2NanoSecond;
}

Time::~Time() = default;

time_t Time::seconds() const {
  return tv_.tv_sec;
}

time_t Time::microSeconds() const {
  return tv_.tv_usec;
}

time_t Time::microStamp() const {
  return tv_.tv_sec * kSecond2MicroSecond + tv_.tv_usec;
}

time_t Time::nanoStamp() const {
  return tv_.tv_sec * kSecond2NanoSecond +
    tv_.tv_usec * kMicroSecond2NanoSecond;
}

string Time::to_string(std::string format) const {
  tm tm{};
  char buf[32];
  gmtime_r(&tv_.tv_sec, &tm);
  strftime(buf, 32, format.c_str(), &tm);
  return {buf};
}

string Time::to_stringWithUs(std::string format) const {
  string now = to_string(format);
  now += "." + std::to_string(tv_.tv_usec);
  return now;
}

timespec Time::to_timespec() const {
  timespec t{};
  t.tv_sec = seconds();
  t.tv_nsec = microSeconds() * kMicroSecond2NanoSecond;
  return t;
}


// set
void Time::setSecond(time_t second) {
  tv_.tv_sec = second;
}

void Time::setMicroSecond(time_t microSecond) {
  tv_.tv_usec = microSecond;
}

void Time::setMax() {
  tv_.tv_sec = std::numeric_limits<long>::max();
  tv_.tv_usec = std::numeric_limits<long>::max();
}


// cal
Time Time::add(Time &right) {
  time_t us = microStamp() + right.microStamp();
  tv_.tv_sec = us / kSecond2MicroSecond;
  tv_.tv_usec = us % kSecond2MicroSecond;
  return *this;
}

Time Time::sub(Time &right) {
  time_t us = microStamp() - right.microStamp();
  tv_.tv_sec = us / kSecond2MicroSecond;
  tv_.tv_usec = us % kSecond2MicroSecond;
  return *this;
}

void Time::addSecond(time_t second) {
  tv_.tv_sec += second;
}

Time Time::operator + (Time &right) {
  time_t us = microStamp() + right.microStamp();
  Time t;
  t.setSecond(us / kSecond2MicroSecond);
  t.setMicroSecond(us % kSecond2MicroSecond);
  return t;
}

Time Time::operator+=(Time &right) {
  return add(right);
}

Time Time::operator-(Time &right) {
  time_t us = microStamp() - right.microStamp();
  Time t;
  t.setSecond(us / kSecond2MicroSecond);
  t.setMicroSecond(us % kSecond2MicroSecond);
  return t;
}

Time Time::operator-=(Time &right) {
  return sub(right);
}

bool Time::operator>(Time &right) {
  return microStamp() - right.microStamp() > 0;
}

bool Time::operator<(Time &right) {
  return microStamp() - right.microStamp() < 0;
}

bool Time::operator==(Time &right) {
  return microStamp() == right.microStamp();
}

// for debug
void Time::show() {
  cout << tv_.tv_sec << " s " << tv_.tv_usec << " us" << endl;
}


// static
time_t Time::microStampMax() {
  return std::numeric_limits<long>::max();
}

time_t Time::nanoStampMax() {
  return std::numeric_limits<long>::max();
}


