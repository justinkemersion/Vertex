cmake_minimum_required(VERSION 3.16)
project(vertex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# doctest for unit testing (header-only, avoid FetchContent CMake compatibility issues)
include(FetchContent)
FetchContent_Declare(
    doctest
    URL https://github.com/doctest/doctest/archive/refs/tags/v2.4.11.tar.gz
)
FetchContent_Populate(doctest)
set(DOCTEST_INCLUDE_DIR ${doctest_SOURCE_DIR})

# ncurses for the game
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

# Library: all game logic (physics, entity, state, input, game, renderer)
add_library(vertex_lib STATIC
    src/Engine.cpp
    src/GameLoop.cpp
    src/entity/Player.cpp
    src/game/GameWorld.cpp
    src/input/CommandQueue.cpp
    src/input/IdleCommand.cpp
    src/input/InputHandler.cpp
    src/input/JumpCommand.cpp
    src/input/MoveLeftCommand.cpp
    src/input/MoveRightCommand.cpp
    src/physics/PhysicsEngine.cpp
    src/renderer/BackBuffer.cpp
    src/renderer/Renderer.cpp
    src/state/IdleState.cpp
    src/state/JumpingState.cpp
    src/state/RunningState.cpp
)

target_include_directories(vertex_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CURSES_INCLUDE_DIRS}
)

if(TARGET Curses::curses)
    target_link_libraries(vertex_lib PUBLIC Curses::curses)
else()
    target_link_libraries(vertex_lib PUBLIC ${CURSES_LIBRARIES})
endif()

# Game executable
add_executable(vertex src/main.cpp)
target_link_libraries(vertex PRIVATE vertex_lib)

# Test executable
add_executable(vertex_tests
    tests/PhysicsEngineTests.cpp
    tests/StateTransitionTests.cpp
)
target_link_libraries(vertex_tests PRIVATE vertex_lib)
target_include_directories(vertex_tests PRIVATE ${CMAKE_SOURCE_DIR}/src ${DOCTEST_INCLUDE_DIR})
