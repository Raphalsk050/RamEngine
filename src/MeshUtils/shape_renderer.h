#pragma once

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <utils/EntityManager.h>
#include <utils/Entity.h>

#include <vector>

namespace utils {
	namespace filamentutils {

		class ShapeRenderer {
		public:
			ShapeRenderer(filament::Engine* engine, filament::Scene* scene);
			~ShapeRenderer();

			utils::Entity createCube(const filament::MaterialInstance* materialInstance);

		private:
			filament::Engine* engine_;
			filament::Scene* scene_;

			filament::VertexBuffer* vertexBufferCube_ = nullptr;
			filament::IndexBuffer* indexBufferCube_ = nullptr;
		};

	} // namespace filamentutils
} // namespace utils

