cmake_minimum_required(VERSION 3.26)
project(lab3Game VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-pthread")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(lab3Game main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

add_subdirectory(source/gameServices)
#add_subdirectory(source/items)
#add_subdirectory(source/inventory)
#add_subdirectory(source/enemy)

set(Qt6_DIR "~/Qt/6.6.0/macos/lib/cmake")
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

#find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

target_include_directories(lab3Game PUBLIC source/gameServices)
target_link_libraries(lab3Game PUBLIC gameServ items inventory enemys)
target_link_libraries(lab3Game PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)
