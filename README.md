## 定时器



参照brpc中的定时器设计，采用hash bucket存储定时器节点以降低临界区以及one timer thread。

requirement:

​	Linux kernel > 2.6.5

​	C++11