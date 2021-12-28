#include <iostream>
#include "src/Time.h"
#include "src/TimerThread.h"
#include <thread>
#include <unistd.h>
#include "src/Timer.h"

using std::cout;
using std::endl;

TimerThread timer;

void timeout_1() {
  //cout << "first time out" << endl;
}

void timeout_2() {
  cout << "time out at:" << Time().to_stringWithUs() << endl;
  cout << "second time out" << endl;
}

void entry() {
  Time now;
  now.addSecond(1);
  cout << "set timer:  " << now.to_stringWithUs() << endl;
  Timer* t = timer.setTimer(timeout_2, now);
}

void entry2() {
  Time now;
  now.addSecond(2);
  Timer* t = timer.setTimer(timeout_2, now);
  sleep(1);
  t->cancel();
}

void entry3() {
  const int size = 10000;
  Timer** arr = new Timer*[size];
  for (int i=0; i<size; i++) {
    Time now;
    now.addSecond(1);
    arr[i] = timer.setTimer(timeout_1, now);
  }
  for (int i=0; i<size; i++) {
    arr[i]->cancel();
  }
}


int main() {
  std::thread t(entry);
  std::thread t2(entry2);
  std::thread t3(entry3);
  t.join();
  t2.join();
  t3.join();
  sleep(3);

}

//int main() {
//  timeval start{};
//  start.tv_sec = 0;
//  start.tv_usec = 1;
//  timeval end{};
//  end.tv_sec = 1;
//  end.tv_usec = 0;
//  Time s(start);
//  Time e(end);
//  Time s2 = s + e;
//  s2.show();
//  s2 += s2;
//  s2.show();
//  s2 -= e;
//  s2.show();
//  cout << Time().to_string() << endl;
//  cout << Time().to_stringWithUs() << endl;
//
//
//}
