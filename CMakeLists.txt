cmake_minimum_required(VERSION 2.8)
project(cpp)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(cpp main.cpp)
target_link_libraries(cpp ${CURSES_LIBRARIES})