cmake_minimum_required(VERSION 3.16)
project(win32_pong LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(win32_pong WIN32
    src/main.cpp
    src/game.cpp
    src/render.cpp
)

target_link_libraries(win32_pong PRIVATE
    d2d1
    dwrite
)

target_compile_definitions(win32_pong PRIVATE UNICODE _UNICODE)
