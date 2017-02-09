cmake_minimum_required(VERSION 2.8.4)

project(gzip_cpp)

# Allow support for C++11
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  set(CMAKE_CXX_STANDARD 11)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

find_package(ZLIB REQUIRED)

include_directories(
  "include"
  "googletest/googletest/include"
  "googletest/googletest/"
  ${ZLIB_INCLUDE_DIRS}
  )

file(GLOB GZIP_SRCS "src/*.cc")
add_library(gzip_cpp ${GZIP_SRCS})

target_link_libraries(gzip_cpp ${ZLIB_LIBRARIES} pthread)

file(GLOB GZIP_TEST_SRCS "test/*.cc")
file(GLOB GOOGLE_TEST_SRCS
  "googletest/googletest/src/gtest-all.cc"
  "googletest/googletest/src/gtest_main.cc"
  )
add_executable(gzip_cpp_test EXCLUDE_FROM_ALL ${GZIP_TEST_SRCS} ${GOOGLE_TEST_SRCS})
target_link_libraries(gzip_cpp_test gzip_cpp)

# Test can be built using 'make test'
add_custom_target(check)
add_dependencies(check gzip_cpp_test)

file(GLOB INCLUDE_FILES "include/*.h")
install(FILES ${INCLUDE_FILES} DESTINATION "include/gzip_cpp")
install(TARGETS gzip_cpp DESTINATION "lib")
