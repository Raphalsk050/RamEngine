#ifndef ENGINE_H
#define ENGINE_H

#include <GLFW/glfw3.h>
#include "../modules/render_module.h" // Certifique-se de incluir o cabeçalho correto

class Engine {
public:
	void init();  // Inicializa a engine
	void run();   // Executa o loop principal

private:
	GLFWwindow* window;
	RenderModule renderModule; // Instância de RenderModule
};

#endif // ENGINE_H