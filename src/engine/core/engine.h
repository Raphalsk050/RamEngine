#ifndef ENGINE_H
#define ENGINE_H

#include <GLFW/glfw3.h>

class Engine {
public:
	void init();  // Inicializa a engine
	void run();   // Executa o loop principal

private:
	GLFWwindow* window;
};

#endif // ENGINE_H