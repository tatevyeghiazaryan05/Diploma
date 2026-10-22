cmake_minimum_required(VERSION 3.16)
project(EdaWindowQuery LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(EdaWindowQuery
    main.cpp
    SpatialIndex.h
    EdaScene.h
    EdaScene.cpp
    MainWindow.h
    MainWindow.cpp
)

target_link_libraries(EdaWindowQuery PRIVATE Qt6::Widgets)