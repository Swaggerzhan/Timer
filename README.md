## 定时器



brpc中的定时器设计，采用hash bucket存储定时器节点以降低临界区以及one timer thread。非常适合rpc的使用，即设定很多定时器的同时定时器比较少会触发。

Install:

```shell
cmake -DCMAKE_INSTALL_PREFIX=路径
```

Example:

```C++
#include "include/TimerThread.h"
#include <iostream>
#include <unistd.h>

void timeoutCB() {
    std::cout << "Timeout!" << std::endl;
}

int main() {
    TimerThead timerThread;
    Time now;
    now.addSecond(1); // timeout after 1 second
    timerThread.setTimer(timeoutCB, now);
    sleep(2);
}
```

Requirement:

```
Linux kernel > 2.6.5
C++11
```

​具体看[Brpc](https://github.com/apache/incubator-brpc/blob/master/src/bthread/timer_thread.cpp)