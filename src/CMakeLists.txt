cmake_minimum_required(VERSION 3.5)

project(tcp_client_lib)

find_package(Qt5 COMPONENTS Core Network Sql WebSockets REQUIRED)

include_directories(${PROJECT_NAME})

set (gtest_disable_pthreads on)

add_subdirectory (googletest)
config_compiler_and_linker()

set(SOURCE_LIB tcp_client.h
               tcp_client.cpp
               instancemessenger.h
               instancemessenger.cpp
               mock_tcp_server.hpp)

add_library(tcp_client_lib STATIC ${SOURCE_LIB})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Network Qt5::Sql Qt5::WebSockets gtest gmock)
