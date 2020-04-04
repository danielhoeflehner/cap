cmake_minimum_required(VERSION 3.17)
set(This ccapTest)

set(SOURCES
  ccap_test.cc
)

add_executable(${This} ${SOURCES})
target_link_libraries(${This} PUBLIC
  gtest_main
  ccap
)

add_test(
  NAME ${This}
  COMMAND ${This}
)

