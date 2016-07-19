cmake_minimum_required(VERSION 2.8.4)

project(gzip_cpp)

include_directories(
  "/usr/local/include"
  "./include"
  "./googletest/googletest/include"
  "googletest/googletest/"
  )

file(GLOB GZIP_SRCS "./src/*.cc")
add_library(gzip_cpp ${GZIP_SRCS})

file(GLOB GZIP_TEST_SRCS "./test/*.cc")
file(GLOB GOOGLE_TEST_SRCS
  "googletest/googletest/src/gtest-all.cc"
  "googletest/googletest/src/gtest_main.cc"
  )
add_executable(gzip_cpp_test ${GZIP_TEST_SRCS} ${GOOGLE_TEST_SRCS})
