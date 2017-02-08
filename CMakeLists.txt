cmake_minimum_required(VERSION 2.8.4)

project(gzip_cpp)

# Allow support for C++11
macro(use_cxx11)
  if(CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    set(CMAKE_CXX_STANDARD 11)
  endif()
endmacro(use_cxx11)

use_cxx11()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")

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
add_executable(gzip_cpp_test ${GZIP_SRCS} ${GZIP_TEST_SRCS} ${GOOGLE_TEST_SRCS})

find_package(ZLIB REQUIRED)
target_link_libraries(gzip_cpp_test ${ZLIB_LIBRARIES} pthread)

file(GLOB INCLUDE_FILES "./include/*.h")
install(FILES ${INCLUDE_FILES} DESTINATION "include/gzip_cpp")
install(TARGETS gzip_cpp DESTINATION "lib")
