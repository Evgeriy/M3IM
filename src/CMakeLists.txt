cmake_minimum_required(VERSION 3.5)

project(tcp_client_lib)

find_package(Qt5 COMPONENTS Core Network Sql WebSockets REQUIRED)

include_directories(${PROJECT_NAME})

set(SOURCE_LIB tcp_client.h
               tcp_client.cpp
               instancemessenger.h
               instancemessenger.cpp
               instancemessenger_global.h

               contacts_model.hpp
               dialogs_model.hpp
               )

add_library(tcp_client_lib STATIC ${SOURCE_LIB})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Network Qt5::Sql Qt5::WebSockets)
