project(enemyLib)

set(SOURCES enemy.cpp)
set(HEADERS enemy.h)

add_subdirectory(/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/source/inventory  ../inventory)

add_library(enemys ${SOURCES} ${HEADERS})
target_include_directories(enemys PUBLIC ../inventory)
target_link_libraries(enemys PUBLIC items inventory)
