//
// Created by swagger on 2021/12/26.
//

#ifndef TIMER_TIME_H
#define TIMER_TIME_H

#include <sys/time.h>
#include <string>

class Time;

void showDiff(Time* start, Time* end);

class Time {
public:

  Time();
  explicit Time(timeval& t);
  // 精度损失
  explicit Time(timespec& t);
  ~Time();

  time_t seconds() const;
  time_t microSeconds() const;

  // 转换为us的时间戳
  time_t microStamp() const;
  // 转为ns的时间戳，精度不够
  time_t nanoStamp() const;
  // 同精度损失
  timespec to_timespec() const;

  // %Y%m%d-%H%M%S
  std::string to_string(std::string format="%Y.%m%.%d-%H:%M:%S") const;
  std::string to_stringWithUs(std::string format="%Y.%m.%d-%H:%M:%S") const;

  // set
  void setSecond(time_t second);
  void setMicroSecond(time_t microSecond);


  // cal
  Time add(Time&);
  Time sub(Time&);
  Time operator+(Time&);
  Time operator+=(Time&);
  Time operator-(Time&);
  Time operator-=(Time&);


  // for debug
  void show();


private:
  timeval tv_;
};



#endif //TIMER_TIME_H
