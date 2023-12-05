project(inventoryLib)

set(SOURCES inventory.cpp)
set(HEADERS inventory.h)

add_subdirectory(/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/source/items  ../items)

add_library(inventory ${SOURCES} ${HEADERS})

target_link_libraries(inventory PRIVATE items)
target_include_directories(inventory PUBLIC ../lib)
