cmake_minimum_required(VERSION 3.26)
project(lab3Game)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab3Game main.cpp
        lib/matrix_lib.h game_lib/BaseItems.cpp game_lib/gameLib.cpp)
