cmake_minimum_required(VERSION 3.20)

project(chip8_emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(SDL2 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    src/*.hpp
    src/*.h
)

add_executable(chip8
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(chip8 PRIVATE
    src
)

target_compile_definitions(chip8 PRIVATE
    SDL_MAIN_HANDLED
)

target_link_libraries(chip8 PRIVATE
    SDL2::SDL2
)