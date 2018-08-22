cmake_minimum_required(VERSION 2.8.12)

project(M3IM)

set(gtest_disable_pthreads on)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(src)
add_subdirectory(src)

find_package(Qt5Core)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Core tcp_client_lib)
