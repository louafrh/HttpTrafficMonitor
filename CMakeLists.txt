cmake_minimum_required(VERSION 3.5)

project(TrafficMonitor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(
        GLOB_RECURSE
        SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/*.cpp
)

file(
        GLOB_RECURSE
        UI_FILES
        ${CMAKE_SOURCE_DIR}/*.ui
)


file(
        GLOB_RECURSE
        INCLUDE_FILES
        ${CMAKE_SOURCE_DIR}/*.h
)

add_executable(TrafficMonitor
    ${SOURCE_FILES}
    ${UI_FILES}
    ${INCLUDE_FILES}
)

target_link_libraries(TrafficMonitor PRIVATE Qt5::Widgets)
