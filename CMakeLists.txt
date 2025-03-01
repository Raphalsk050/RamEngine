cmake_minimum_required(VERSION 3.10)
project(RamEngine)

# Definir o padrão C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar GLFW e OpenGL
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Adicionar o diretório src
add_subdirectory(src)