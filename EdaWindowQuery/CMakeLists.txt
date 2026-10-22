cmake_minimum_required(VERSION 3.16)
project(EdaWindowQuery LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(EdaWindowQuery
    main.cpp
    SpatialIndex.h
    EdaScene.h
    EdaScene.cpp
    MainWindow.h
    MainWindow.cpp
)

target_link_libraries(EdaWindowQuery PRIVATE 
    Qt6::Widgets
    Qt6::Core 
    Qt6::Gui
)

target_include_directories(EdaWindowQuery PRIVATE
    /usr/include/x86_64-linux-gnu/qt6
    /usr/include/x86_64-linux-gnu/qt6/QtCore
    /usr/include/x86_64-linux-gnu/qt6/QtGui
    /usr/include/x86_64-linux-gnu/qt6/QtWidgets
)