cmake_minimum_required(VERSION 3.16)
project(vertex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(vertex ${SOURCES})

target_include_directories(vertex PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CURSES_INCLUDE_DIRS}
)

if(TARGET Curses::curses)
    target_link_libraries(vertex PRIVATE Curses::curses)
else()
    target_link_libraries(vertex PRIVATE ${CURSES_LIBRARIES})
endif()
