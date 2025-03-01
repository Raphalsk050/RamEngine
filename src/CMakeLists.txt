# Criar o executável
add_executable(ramEngine main.cpp)
target_link_libraries(ramEngine PRIVATE engine_core)

# Linkar GLFW e OpenGL
target_link_libraries(ramEngine PRIVATE glfw)
target_link_libraries(ramEngine PRIVATE engine_core render_module OpenGL::GL)

# Adicionar o diretório engine
add_subdirectory(engine)