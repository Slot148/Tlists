cmake_minimum_required(VERSION 3.10)
project(tlist VERSION 1.0.3 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(COMPILE_FLAGS -Wall -Wextra -Wpedantic)

add_library(Tlist STATIC 
    src/Tlist.c 
    src/Titerator.c 
    src/Tvector.c
    src/Ttree.c
)
    
target_compile_options(Tlist PRIVATE ${COMPILE_FLAGS})
target_include_directories(Tlist PUBLIC include/tlist/)
add_executable(main main.c)
target_link_libraries(main PRIVATE Tlist)