#include "engine.h"
#include "modules/render_module.h"
#include <iostream>

void Engine::init() {
	// Inicializar GLFW
	if (!glfwInit()) {
		std::cerr << "Falha ao inicializar GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Criar janela
	window = glfwCreateWindow(800, 600, "RamEngine", NULL, NULL);
	if (!window) {
		std::cerr << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Definir contexto OpenGL
	glfwMakeContextCurrent(window);

	// Inicializar módulo de renderização
	RenderModule::init();
}

void Engine::run() {
	while (!glfwWindowShouldClose(window)) {
		// Renderizar a cena
		RenderModule::render();

		// Trocar buffers e processar eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Finalizar
	glfwDestroyWindow(window);
	glfwTerminate();
}