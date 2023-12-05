cmake_minimum_required(VERSION 3.26)
project(lab3Game VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)



add_executable(lab3Game main.cpp)

add_subdirectory(source/gameServices)
#add_subdirectory(source/items)
#add_subdirectory(source/inventory)
#add_subdirectory(source/enemy)
target_include_directories(lab3Game PUBLIC source/gameServices)
target_link_libraries(lab3Game PUBLIC gameServ items inventory enemys)



