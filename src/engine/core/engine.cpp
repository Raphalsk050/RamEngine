#include "engine.h"
#include <iostream>

void Engine::init() {
	// Inicializar GLFW
	if (!glfwInit()) {
		std::cerr << "Falha ao inicializar GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Criar janela
	window = glfwCreateWindow(800, 600, "Minha Engine", NULL, NULL);
	if (!window) {
		std::cerr << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Definir contexto OpenGL
	glfwMakeContextCurrent(window);

	// Inicializar módulo de renderização usando a instância
	renderModule.init();
}

void Engine::run() {
	while (!glfwWindowShouldClose(window)) {
		// Renderizar a cena usando a instância
		renderModule.render();

		// Trocar buffers e processar eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Finalizar
	glfwDestroyWindow(window);
	glfwTerminate();
}