cmake_minimum_required(VERSION 2.8.4)
project(buffer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11 -stdlib=libc++")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")

include_directories("${CMAKE_SOURCE_DIR}/include")
add_subdirectory(src)