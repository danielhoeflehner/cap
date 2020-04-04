cmake_minimum_required(VERSION 3.17)
project(ccap VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

enable_testing()
add_subdirectory(googletest)

set(SOURCES
  example.cc
)

add_library(ccap INTERFACE)
add_subdirectory(test)

add_executable(example ${SOURCES})
target_link_libraries(example PRIVATE ccap)

