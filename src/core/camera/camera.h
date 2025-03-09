#ifndef CAMERA_H
#define CAMERA_H

#include <filament/Camera.h>
#include <filament/Engine.h>
#include <filament/Scene.h>
#include <filament/math/vec3.h>

class Camera {
public:
	// Construtor que inicializa a câmera com o motor e a cena do Filament
	Camera(filament::Engine* engine, filament::Scene* scene);

	// Atualiza a câmera (pode ser usado para animações ou lógica contínua)
	void update(float deltaTime);

	// Processa o movimento do cursor para rotacionar a câmera
	void onCursorMove(double xpos, double ypos);

	// Define a posição da câmera
	void setPosition(const filament::math::float3& position);

	// Define o vetor "up" da câmera
	void setUp(const filament::math::float3& up);

private:
	filament::Camera* camera_;           // Ponteiro para a câmera do Filament
	filament::math::float3 position_;    // Posição da câmera
	filament::math::float3 up_;          // Vetor "up" da câmera
	double lastX_;                       // Última posição X do cursor
	double lastY_;                       // Última posição Y do cursor
	bool firstMouse_;                    // Flag para o primeiro movimento do mouse
	float yaw_;                          // Ângulo de rotação horizontal
	float pitch_;                        // Ângulo de rotação vertical
	float sensitivity_;                  // Sensibilidade do movimento
};

#endif // CAMERA_H