#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <functional>

class Input {
public:
	// Tipo de callback para a posição do cursor
	using CursorPosCallback = std::function<void(double, double)>;

	// Construtor que recebe a janela GLFW
	Input(GLFWwindow* window);

	// Define o callback para o movimento do cursor
	void setCursorPosCallback(CursorPosCallback callback);

	// Processa os eventos de entrada
	void pollEvents();

	// Inicializa o sistema de entrada
	void initialize();

private:
	GLFWwindow* window_;              // Ponteiro para a janela GLFW
	CursorPosCallback cursorPosCallback_;  // Callback para posição do cursor

	// Função estática de callback para o GLFW
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	// Configura o ponteiro do usuário na janela GLFW
	void setUserPointer();
};

#endif // INPUT_H