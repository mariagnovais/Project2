cmake_minimum_required(VERSION 3.26)
project(Project2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_DIR "C:/Libraries/SFML-2.5.1/lib/cmake/SFML")
# find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(Project2
        src/main.cpp
        src/hashmap_of_hashmaps.cpp
        src/helpers.cpp
        src/Heap.cpp
)

target_include_directories(Project2 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        sfml-graphics
)