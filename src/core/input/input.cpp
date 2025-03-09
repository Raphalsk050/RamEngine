#include "input.h"

Input::Input(GLFWwindow* window) : window_(window) {
	// Registra o callback de posição do cursor no GLFW
	glfwSetCursorPosCallback(window, cursor_position_callback);
}

void Input::setCursorPosCallback(CursorPosCallback callback) {
	cursorPosCallback_ = callback;
}

void Input::pollEvents() {
	// Processa os eventos da janela
	glfwPollEvents();
}

void Input::initialize() {
	// Configura o ponteiro do usuário
	setUserPointer();
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	// Recupera o objeto Input associado à janela
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
	if (input && input->cursorPosCallback_) {
		// Chama o callback com as coordenadas do cursor
		input->cursorPosCallback_(xpos, ypos);
	}
}

void Input::setUserPointer() {
	// Associa este objeto Input ao ponteiro de usuário da janela
	glfwSetWindowUserPointer(window_, this);
}