cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(cash_register ${CMAKE_SOURCE_DIR}/src/main.cpp)
