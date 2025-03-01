#include "render_module.h"
#include <GL/gl.h>

void RenderModule::init() {
	// Configurações iniciais do OpenGL, se necessário
}

void RenderModule::render() {
	// Limpar a tela com uma cor de fundo
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Desenhar um triângulo (exemplo)
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glEnd();
}