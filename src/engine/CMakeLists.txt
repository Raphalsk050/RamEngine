# Biblioteca para o núcleo
add_library(engine_core STATIC core/engine.cpp)

# Módulo de renderização
add_library(render_module STATIC modules/render_module.cpp)
target_link_libraries(render_module PRIVATE engine_core)

# Linkar tudo ao executável
target_link_libraries(ramEngine PRIVATE engine_core render_module)