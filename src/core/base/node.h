#ifndef NODE_H
#define NODE_H

#include <filament/Engine.h>
#include <filament/TransformManager.h>
#include <utils/Entity.h>
#include <math/vec3.h>
#include <math/quat.h>
#include <math/mat4.h>

using namespace filament;

class Node {
public:
	// Construtor que associa uma entidade ao nó
	explicit Node(Engine *engine);

	~Node();

	// Define a posição do nó
	void setPosition(const math::float3 &position);

	// Define a rotação do nó
	void setRotation(const filament::math::quat &rotation);

	// Define a escala do nó
	void setScale(const filament::math::float3 &scale);

	[[nodiscard]] utils::Entity getEntity() const;

private:
	Engine *engine_; // Ponteiro para o motor do Filament
	utils::Entity entity_; // Entidade associada ao nó
	TransformManager *transformManager_; // Gerenciador de transformações
	TransformManager::Instance instance_; // Instância da transformação
};

#endif // NODE_H
