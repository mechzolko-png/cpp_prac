cmake_minimum_required(VERSION 3.20)

project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW beállítások
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# GLFW hozzáadása
add_subdirectory(
    "C:/Users/Zotyi/glfw-3.5.1/glfw-3.5.1"
    glfw-build
)

# Saját játék
add_executable(MyGame g.cpp)

# GLFW + Windows OpenGL
target_link_libraries(MyGame
    PRIVATE
    glfw
    opengl32
)