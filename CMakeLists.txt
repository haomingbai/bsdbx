cmake_minimum_required(VERSION 3.10.0)
project(bsdbx VERSION 0.1.0 LANGUAGES C CXX)

add_executable(bsdbx executable.cpp rule.h)
target_link_libraries(bsdbx PRIVATE seccomp)
