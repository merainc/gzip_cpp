cmake_minimum_required(CMAKE 2.8.4)

project(gzip_cpp)

include_directories("/usr/local/include", "./include")

file(GLOB GZIP_SRCS "./src/*.cc")
add_library(gzip_cpp ${GZIP_SRCS})

file(GLOB GZIP_TEST_SRCS "./test/*.cc")
add_executable(gzip_cpp_test ${GZIP_TEST_SRCS})
