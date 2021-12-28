cmake_minimum_required(VERSION 3.10)
project(Timer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE DEBUG)

find_package(Threads)

add_executable(Timer main.cpp
        src/Time.h src/Time.cpp
        src/futex.h src/futex.cpp
        src/TimerThread.h src/TimerThread.cpp
        src/Time.h src/Time.cpp
        src/Timer.h src/Timer.cpp
        )

target_link_libraries(Timer Threads::Threads)