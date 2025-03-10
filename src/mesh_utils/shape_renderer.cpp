#include "shape_renderer.h"

#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <utils/EntityManager.h>
#include <utils/Entity.h>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>

using namespace filament;
using namespace utils;

struct Vertex {
	filament::math::float2 position;
	uint32_t color;
};

static const Vertex TRIANGLE_VERTICES[3] = {
	{{1, 0}, 0xffff0000u},
	{{cos(M_PI * 2 / 3), sin(M_PI * 2 / 3)}, 0xff00ff00u},
	{{cos(M_PI * 4 / 3), sin(M_PI * 4 / 3)}, 0xff0000ffu},
};

static constexpr uint16_t TRIANGLE_INDICES[3] = {0, 1, 2};

namespace utils {
	namespace filamentutils {
		ShapeRenderer::ShapeRenderer(Engine *engine, Scene *scene) : engine_(engine), scene_(scene) {
			// --- Dados do Cubo ---
			float verticesCube[] = {
				// Frente
				-0.5f, -0.5f, 0.5f, // 0
				0.5f, -0.5f, 0.5f, // 1
				0.5f, 0.5f, 0.5f, // 2
				-0.5f, 0.5f, 0.5f, // 3
				// Trás
				-0.5f, -0.5f, -0.5f, // 4
				0.5f, -0.5f, -0.5f, // 5
				0.5f, 0.5f, -0.5f, // 6
				-0.5f, 0.5f, -0.5f // 7
			};
			uint16_t indicesCube[] = {
				// Frente
				0, 1, 2, 2, 3, 0,
				// Direita
				1, 5, 6, 6, 2, 1,
				// Trás
				5, 4, 7, 7, 6, 5,
				// Esquerda
				4, 0, 3, 3, 7, 4,
				// Topo
				3, 2, 6, 6, 7, 3,
				// Fundo
				4, 5, 1, 1, 0, 4
			};

			// --- Criar VertexBuffer e IndexBuffer para o Cubo ---
			vertexBufferCube_ = VertexBuffer::Builder()
					.vertexCount(8)
					.bufferCount(1)
					.attribute(VertexAttribute::POSITION, 0, VertexBuffer::AttributeType::FLOAT3, 0, 12)
					.build(*engine_);
			vertexBufferCube_->setBufferAt(*engine_, 0,
			                               VertexBuffer::BufferDescriptor(verticesCube, sizeof(verticesCube)));

			indexBufferCube_ = IndexBuffer::Builder()
					.indexCount(36)
					.bufferType(IndexBuffer::IndexType::USHORT)
					.build(*engine_);
			indexBufferCube_->setBuffer(*engine_, IndexBuffer::BufferDescriptor(indicesCube, sizeof(indicesCube)));

			// --- Criar VertexBuffer e IndexBuffer para Linhas ---
			vertexBufferLine_ = VertexBuffer::Builder()
					.vertexCount(2) // Inicialmente para uma linha, será atualizado dinamicamente
					.bufferCount(1)
					.attribute(VertexAttribute::POSITION, 0, VertexBuffer::AttributeType::FLOAT3, 0, 28)
					.attribute(VertexAttribute::COLOR, 0, VertexBuffer::AttributeType::FLOAT4, 12, 28)
					.build(*engine_);

			indexBufferLine_ = IndexBuffer::Builder()
					.indexCount(2) // Inicialmente para uma linha
					.bufferType(IndexBuffer::IndexType::USHORT)
					.build(*engine_);

			// --- Carregar Material Padrão para Linhas ---
			std::ifstream materialFileStream("compiled_materials/simple_material.filamat",
			                                 std::ios::binary | std::ios::in);
			if (!materialFileStream.is_open()) {
				SDL_Log("ERRO: Falha ao abrir o arquivo .filamat do material padrão para linhas!");
				return; // Tratamento básico de erro
			}
			materialFileStream.seekg(0, std::ifstream::end);
			size_t materialFileSize = materialFileStream.tellg();
			materialFileStream.seekg(0, std::ifstream::beg);
			char *materialData = new char[materialFileSize];
			materialFileStream.read(materialData, materialFileSize);
			materialFileStream.close();

			Material *material = Material::Builder()
					.package(materialData, materialFileSize)
					.build(*engine_);
			delete[] materialData;

			if (material) {
				defaultLineMaterial_ = material->createInstance();
				defaultLineMaterial_->setParameter("BaseColor", math::float4(0.0f, 1.0f, 0.0f, 1.0f)); // Verde
			} else {
				SDL_Log("ERRO: Falha ao criar instância do material padrão para linhas!");
			}
		}

		ShapeRenderer::~ShapeRenderer() {
			if (vertexBufferCube_) engine_->destroy(vertexBufferCube_);
			if (indexBufferCube_) engine_->destroy(indexBufferCube_);
			if (vertexBufferLine_) engine_->destroy(vertexBufferLine_);
			if (indexBufferLine_) engine_->destroy(indexBufferLine_);
			if (defaultLineMaterial_) engine_->destroy(defaultLineMaterial_);
		}

		Node *ShapeRenderer::createCube(const MaterialInstance *materialInstance) {
			Node *node = new Node(engine_);
			RenderableManager::Builder(1) // 1 geometria
					.geometry(0, RenderableManager::PrimitiveType::TRIANGLES, vertexBufferCube_, indexBufferCube_, 0,
					          36)
					.material(0, materialInstance)
					.build(*engine_, node->getEntity());
			scene_->addEntity(node->getEntity());
			return node;
		}

		Node *ShapeRenderer::createLine(const math::float3 &p1, const math::float3 &p2,
		                                const MaterialInstance *materialInstance) {
			// Configurar os dados da linha
			float vertices[] = {
				p1.x, p1.y, p1.z, 1.0f, 1.0f, 1.0f, 1.0f, // Ponto 1 (x, y, z, r, g, b, a)
				p2.x, p2.y, p2.z, 1.0f, 1.0f, 1.0f, 1.0f // Ponto 2
			};
			uint16_t indices[] = {0, 1};

			vertexBufferLine_->setBufferAt(*engine_, 0, VertexBuffer::BufferDescriptor(vertices, sizeof(vertices)));
			indexBufferLine_->setBuffer(*engine_, IndexBuffer::BufferDescriptor(indices, sizeof(indices)));

			Node *node = new Node(engine_);
			RenderableManager::Builder(1)
					.geometry(0, RenderableManager::PrimitiveType::LINES, vertexBufferLine_, indexBufferLine_, 0, 2)
					.material(0, materialInstance)
					.build(*engine_, node->getEntity());
			scene_->addEntity(node->getEntity());
			return node;
		}

		Node *ShapeRenderer::createLineList(const std::vector<math::float3> &points,
		                                    const MaterialInstance *materialInstance) {
			if (points.size() < 2) return nullptr;

			std::vector<float> vertices;
			std::vector<uint16_t> indices;
			for (size_t i = 0; i < points.size(); ++i) {
				vertices.push_back(points[i].x);
				vertices.push_back(points[i].y);
				vertices.push_back(points[i].z);
				vertices.push_back(1.0f); // r
				vertices.push_back(1.0f); // g
				vertices.push_back(1.0f); // b
				vertices.push_back(1.0f); // a
				if (i > 0) {
					indices.push_back(i - 1);
					indices.push_back(i);
				}
			}

			vertexBufferLine_->setBufferAt(*engine_, 0,
			                               VertexBuffer::BufferDescriptor(
				                               vertices.data(), vertices.size() * sizeof(float)));
			indexBufferLine_->setBuffer(
				*engine_, IndexBuffer::BufferDescriptor(indices.data(), indices.size() * sizeof(uint16_t)));

			Node *node = new Node(engine_);
			RenderableManager::Builder(1)
					.geometry(0, RenderableManager::PrimitiveType::LINES, vertexBufferLine_, indexBufferLine_, 0,
					          indices.size())
					.material(0, materialInstance)
					.build(*engine_, node->getEntity());
			scene_->addEntity(node->getEntity());
			return node;
		}

		Node *ShapeRenderer::createLine(const math::float3 &p1, const math::float3 &p2) {
			if (!defaultLineMaterial_) {
				SDL_Log("AVISO: Material padrão para linhas não inicializado. Impossível criar linha de debug.");
				return nullptr; // Retorna entidade nula
			}
			return createLine(p1, p2, defaultLineMaterial_);
		}

		Node *ShapeRenderer::createTriangle() {
			auto vb = VertexBuffer::Builder()
					.vertexCount(3)
					.bufferCount(1)
					.attribute(VertexAttribute::POSITION, 0, VertexBuffer::AttributeType::FLOAT2, 0, 12)
					.attribute(VertexAttribute::COLOR, 0, VertexBuffer::AttributeType::UBYTE4, 8, 12)
					.normalized(VertexAttribute::COLOR)
					.build(*engine_);
			vb->setBufferAt(*engine_, 0,
			                VertexBuffer::BufferDescriptor(TRIANGLE_VERTICES, 36, nullptr));
			auto ib = IndexBuffer::Builder()
					.indexCount(3)
					.bufferType(IndexBuffer::IndexType::USHORT)
					.build(*engine_);
			ib->setBuffer(*engine_,
			              IndexBuffer::BufferDescriptor(TRIANGLE_INDICES, 6, nullptr));

			Node *node = new Node(engine_);
			RenderableManager::Builder(1)
					.boundingBox({{-1, -1, -1}, {1, 1, 1}})
					.material(0, defaultLineMaterial_)
					.geometry(0, RenderableManager::PrimitiveType::TRIANGLES, vb, ib, 0, 3)
					.culling(false)
					.receiveShadows(false)
					.castShadows(false)
					.build(*engine_, node->getEntity());
			return node;
		}

		Node *ShapeRenderer::createLineList(const std::vector<math::float3> &points) {
			if (!defaultLineMaterial_) {
				SDL_Log(
					"AVISO: Material padrão para linhas não inicializado. Impossível criar lista de linhas de debug.");
				return nullptr; // Retorna entidade nula
			}
			return createLineList(points, defaultLineMaterial_);
		}
	} // namespace filamentutils
} // namespace utils
