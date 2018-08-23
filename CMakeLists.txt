cmake_minimum_required(VERSION 3.5)

project(M3IM)

option(Build_Src "Build source lib" ON)
option(Build_Unit_Tests "Build unit tests" ON)

set(gtest_disable_pthreads on)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

add_executable(${PROJECT_NAME} "main.cpp")

if (Build_Src)
    include_directories(src)
    add_subdirectory(src)
    target_link_libraries(${PROJECT_NAME} tcp_client_lib)
endif()

if (Build_Unit_Tests)
    include_directories(tests)
    add_subdirectory(tests)
    target_link_libraries(${PROJECT_NAME} unit_tests)
endif()


target_link_libraries(${PROJECT_NAME} Qt5::Core)



