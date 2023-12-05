project(itemsLib)

set(SOURCES items.cpp
        baseItems.cpp)
set(HEADERS items.h)

add_library(items ${SOURCES} ${HEADERS})
target_include_directories(items PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})