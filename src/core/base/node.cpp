#include "node.h"
#include <utils/EntityManager.h>

Node::Node(Engine* engine)
	: engine_(engine) {
	// Cria a entidade internamente
	entity_ = utils::EntityManager::get().create();
	transformManager_ = &engine->getTransformManager();
	// Cria uma instância de transformação para a entidade
	transformManager_->create(entity_, instance_);
}

Node::~Node() {
	// Destroi a instância de transformação
	engine_->destroy(entity_);
	// Destroi a entidade
	utils::EntityManager::get().destroy(entity_);
}

void Node::setPosition(const math::float3& position) {
	// Obtém a matriz de transformação atual
	auto transform = transformManager_->getTransform(instance_);
	// Atualiza a posição
	transform[3].xyz = position;
	transformManager_->setTransform(instance_, transform);
}

void Node::setRotation(const math::quat& rotation) {
	// Obtém a matriz de transformação atual
	auto transform = transformManager_->getTransform(instance_);
	// Converte o quaternion em uma matriz de rotação
	math::mat3 rotationMatrix(rotation);
	transform[0].xyz = rotationMatrix[0];
	transform[1].xyz = rotationMatrix[1];
	transform[2].xyz = rotationMatrix[2];
	transformManager_->setTransform(instance_, transform);
}

void Node::setScale(const math::float3& scale) {
	// Obtém a matriz de transformação atual
	auto transform = transformManager_->getTransform(instance_);
	// Aplica a escala aos eixos
	transform[0].xyz *= scale.x;
	transform[1].xyz *= scale.y;
	transform[2].xyz *= scale.z;
	transformManager_->setTransform(instance_, transform);
}

// Opcional: método para acessar a entidade de forma controlada
utils::Entity Node::getEntity() const {
	return entity_;
}