#include "camera.h"
#include <cmath>

Camera::Camera(filament::Engine* engine, filament::Scene* scene) {
	camera_ = engine->createCamera();
	scene->setCamera(camera_);
	position_ = {0.0f, 0.0f, 5.0f};  // Posição inicial
	up_ = {0.0f, 1.0f, 0.0f};        // Vetor "up" padrão
	lastX_ = 0.0;
	lastY_ = 0.0;
	firstMouse_ = true;
	yaw_ = 0.0f;
	pitch_ = 0.0f;
	sensitivity_ = 0.1f;              // Sensibilidade padrão
}

void Camera::update(float deltaTime) {
	// Lógica de atualização contínua pode ser adicionada aqui, se necessário
}

void Camera::onCursorMove(double xpos, double ypos) {
	if (firstMouse_) {
		lastX_ = xpos;
		lastY_ = ypos;
		firstMouse_ = false;
	}

	// Calcula o deslocamento do cursor
	double xoffset = xpos - lastX_;
	double yoffset = lastY_ - ypos;  // Invertido porque o eixo Y cresce para baixo

	lastX_ = xpos;
	lastY_ = ypos;

	// Aplica a sensibilidade
	xoffset *= sensitivity_;
	yoffset *= sensitivity_;

	// Atualiza os ângulos yaw e pitch
	yaw_ += xoffset;
	pitch_ += yoffset;

	// Limita o pitch para evitar giro completo
	if (pitch_ > 89.0f) pitch_ = 89.0f;
	if (pitch_ < -89.0f) pitch_ = -89.0f;

	// Calcula a direção da câmera
	filament::math::float3 front;
	front.x = cos(filament::math::radians(yaw_)) * cos(filament::math::radians(pitch_));
	front.y = sin(filament::math::radians(pitch_));
	front.z = sin(filament::math::radians(yaw_)) * cos(filament::math::radians(pitch_));

	// Atualiza a orientação da câmera
	camera_->lookAt(position_, position_ + front, up_);
}

void Camera::setPosition(const filament::math::float3& position) {
	position_ = position;
	camera_->setPosition(position_);
}

void Camera::setUp(const filament::math::float3& up) {
	up_ = up;
}