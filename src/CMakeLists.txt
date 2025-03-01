# Criar o executável
add_executable(ramEngine main.cpp)

# Linkar GLFW e OpenGL
target_link_libraries(ramEngine PRIVATE glfw OpenGL::GL)

# Adicionar o diretório engine
add_subdirectory(engine)