cmake_minimum_required(VERSION 3.5)

project(M3IM)

option(Build_Src "Build source lib" ON)
option(Build_Unit_Tests "Build unit tests" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Network Quick REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp" "qml.qrc")

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

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Quick)



