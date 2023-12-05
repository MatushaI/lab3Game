project(gamesLib)

set(SOURCES gameServices.cpp)
set(HEADERS gameServices.h)

add_subdirectory(/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/source/enemy  ../enemy)

add_library(gameServ ${SOURCES} ${HEADERS})
target_include_directories(gameServ PUBLIC ../inventory)
target_include_directories(gameServ PUBLIC ../enemy)
target_include_directories(gameServ PUBLIC ../lib)
target_link_libraries(gameServ PUBLIC items enemys)