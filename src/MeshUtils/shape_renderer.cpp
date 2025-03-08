#include "shape_renderer.h"

using namespace filament;
using namespace utils;

namespace utils {
	namespace filamentutils {
		ShapeRenderer::ShapeRenderer(Engine *engine, Scene *scene) : engine_(engine), scene_(scene) {
			// --- Dados do Cubo ---
			float vertices[] = {
				// Frente
				-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
				// Trás
				-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
				// Esquerda
				-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
				// Direita
				0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
				// Topo
				-0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
				// Base
				-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
			};

			uint16_t indices[] = {
				// Frente
				0, 1, 2, 2, 3, 0,
				// Trás
				4, 5, 6, 6, 7, 4,
				// Esquerda
				8, 9, 10, 10, 11, 8,
				// Direita
				12, 13, 14, 14, 15, 12,
				// Topo
				16, 17, 18, 18, 19, 16,
				// Base
				20, 21, 22, 22, 23, 20
			};

			// Criar VertexBuffer para o cubo (compartilhado entre todas as instâncias de cubo)
			vertexBufferCube_ = VertexBuffer::Builder()
					.vertexCount(24)
					.bufferCount(1)
					.attribute(VertexAttribute::POSITION, 0, VertexBuffer::AttributeType::FLOAT3, 0, 0)
					.build(*engine_);

			vertexBufferCube_->setBufferAt(*engine_, 0,
			                               VertexBuffer::BufferDescriptor(vertices, sizeof(vertices)));

			// Criar IndexBuffer para o cubo (compartilhado entre todas as instâncias de cubo)
			indexBufferCube_ = IndexBuffer::Builder()
					.indexCount(36)
					// .indexType(IndexBuffer::IndexType::USHORT) // Comentado ou REMOVIDO
					.build(*engine_);

			indexBufferCube_->setBuffer(*engine_,
			                            IndexBuffer::BufferDescriptor(indices, sizeof(indices)));
		}

		ShapeRenderer::~ShapeRenderer() {
			// Limpar os recursos compartilhados do cubo
			if (vertexBufferCube_) engine_->destroy(vertexBufferCube_);
			if (indexBufferCube_) engine_->destroy(indexBufferCube_);
		}


		Entity ShapeRenderer::createCube(const MaterialInstance *materialInstance) {
			Entity cubeEntity = EntityManager::get().create();

			RenderableManager::Builder(1)
					.castShadows(true)
					.receiveShadows(true)
					.geometry(0, RenderableManager::PrimitiveType::TRIANGLES, vertexBufferCube_, indexBufferCube_, 0,
					          36)
					.material(0, materialInstance)
					.build(*engine_, cubeEntity);

			scene_->addEntity(cubeEntity);
			return cubeEntity;
		}
	} // namespace filamentutils
} // namespace utils
