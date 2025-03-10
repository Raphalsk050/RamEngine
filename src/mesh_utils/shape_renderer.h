#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <utils/Entity.h>
#include <vector>

#include "src/core/base/node.h"

using namespace filament;

namespace utils {
	namespace filamentutils {
		class ShapeRenderer {
		public:
			ShapeRenderer(filament::Engine *engine, filament::Scene *scene);

			~ShapeRenderer();

			Node *createCube(const filament::MaterialInstance *materialInstance);

			Node *createLine(const math::float3 &p1, const math::float3 &p2,
			                 const filament::MaterialInstance *materialInstance); // Método com material (existente)
			Node *createLine(const math::float3 &p1, const math::float3 &p2);

			Node* createTriangle();

			// Novo método para linha de debug (sem material)
			Node *createLineList(const std::vector<math::float3> &points,
			                     const filament::MaterialInstance *materialInstance); // Método com material (existente)
			Node *createLineList(const std::vector<math::float3> &points);

			// Novo método para lista de linhas de debug (sem material)


		private:
			filament::Engine *engine_;
			filament::Scene *scene_;

			filament::VertexBuffer *vertexBufferCube_ = nullptr;
			filament::IndexBuffer *indexBufferCube_ = nullptr;
			filament::VertexBuffer *vertexBufferLine_ = nullptr;
			filament::IndexBuffer *indexBufferLine_ = nullptr;
			filament::MaterialInstance *defaultLineMaterial_ = nullptr; // Material padrão para linhas de debug (NOVO)
		};
	} // namespace filamentutils
} // namespace utils

#endif // SHAPE_RENDERER_H
