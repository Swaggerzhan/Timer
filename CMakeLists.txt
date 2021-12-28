cmake_minimum_required(VERSION 3.10)
project(timer)

set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_CXX_STANDARD 14)

add_library(timer
            src/futex.h src/futex.cpp
            src/Timer.h src/Timer.cpp
            src/Time.h src/Time.cpp
            src/TimerThread.h src/TimerThread.cpp
        )


set(INSTALL_HEAD_FILES src/TimerThread.h src/Timer.h src/Time.h)

install(
        TARGETS timer
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

install(
        FILES ${INSTALL_HEAD_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)