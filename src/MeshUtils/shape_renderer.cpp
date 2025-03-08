#include "shape_renderer.h"

#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <utils/EntityManager.h>
#include <utils/Entity.h>
#include <vector>
#include <fstream> // Para ifstream - necessário para carregar o material padrão

#include <SDL2/SDL.h> // Para SDL_Log - pode remover se tiver outra forma de log

using namespace filament;
using namespace utils;

namespace utils {
	namespace filamentutils {
		ShapeRenderer::ShapeRenderer(Engine *engine, Scene *scene) : engine_(engine), scene_(scene) {
			// --- Dados do Cubo --- (já existente - não altere)
			float verticesCube[] = {
				/* ... vertices do cubo ... */
			}; // (Código do cubo anterior - não altere)
			uint16_t indicesCube[] = {
				/* ... indices do cubo ... */
			}; // (Código do cubo anterior - não altere)

			// --- Dados da Linha (já existente - não altere) ---
			vertexBufferLine_ = VertexBuffer::Builder() /* ... */.build(*engine_); // (Código anterior - não altere)
			indexBufferLine_ = IndexBuffer::Builder() /* ... */.build(*engine_); // (Código anterior - não altere)
			vertexBufferLine_->setBufferAt(*engine_, 0, VertexBuffer::BufferDescriptor(nullptr, 0));
			// (Código anterior - não altere)
			indexBufferLine_->setBuffer(*engine_, IndexBuffer::BufferDescriptor(nullptr, 0));
			// (Código anterior - não altere)


			// --- Criar VertexBuffer e IndexBuffer para o Cubo (já existente - não altere) ---
			vertexBufferCube_ = VertexBuffer::Builder() /* ... */.build(*engine_); // (Código anterior - não altere)
			vertexBufferCube_->setBufferAt(*engine_, 0,
			                               VertexBuffer::BufferDescriptor(verticesCube, sizeof(verticesCube)));
			// (Código anterior - não altere)
			indexBufferCube_ = IndexBuffer::Builder() /* ... */.build(*engine_); // (Código anterior - não altere)
			indexBufferCube_->setBuffer(*engine_, IndexBuffer::BufferDescriptor(indicesCube, sizeof(indicesCube)));
			// (Código anterior - não altere)


			// --- Criar Material Padrão para Linhas de Debug (NOVO) ---
			std::ifstream materialFileStream("PATH_TO_YOUR_UNLIT_COLOR.filamat", std::ios::binary | std::ios::in);
			// <-- ***SUBSTITUA PELO CAMINHO DO SEU ARQUIVO .filamat***
			if (!materialFileStream.is_open()) {
				SDL_Log("ERRO: Falha ao abrir o arquivo .filamat do material padrão para linhas!");
				// ***TODO:***  Tratamento de erro mais robusto aqui, talvez retornar um erro ou usar um material fallback embutido.
				return; // Retorna sem criar o material se o arquivo não for encontrado (tratamento de erro básico)
			}
			materialFileStream.seekg(0, std::ifstream::end);
			size_t materialFileSize = materialFileStream.tellg();
			materialFileStream.seekg(0, std::ifstream::beg);
			char *materialData = new char[materialFileSize];
			materialFileStream.read(materialData, materialFileSize);
			materialFileStream.close();

			Material *material = Material::Builder()
					.build(*engine_);
			delete[] materialData;

			if (material) {
				defaultLineMaterial_ = material->createInstance();
				defaultLineMaterial_->setParameter("baseColor", math::float4(0.0f, 1.0f, 0.0f, 1.0f));
				// Cor verde padrão
			} else {
				SDL_Log("ERRO: Falha ao criar instância do material padrão para linhas!");
				// ***TODO:*** Tratamento de erro mais robusto - o que fazer se o material não for criado? Usar um fallback?
			}
		}

		ShapeRenderer::~ShapeRenderer() {
			// Limpar recursos do cubo (já existente - não altere)
			if (vertexBufferCube_) engine_->destroy(vertexBufferCube_);
			if (indexBufferCube_) engine_->destroy(indexBufferCube_);

			// Limpar recursos de linha (já existente - não altere)
			if (vertexBufferLine_) engine_->destroy(vertexBufferLine_);
			if (indexBufferLine_) engine_->destroy(indexBufferLine_);

			// Limpar Material Padrão para Linhas de Debug (NOVO)
			if (defaultLineMaterial_) engine_->destroy(defaultLineMaterial_);
		}


		Entity ShapeRenderer::createCube(const MaterialInstance *materialInstance) {
			/* ... código createCube já existente - não altere ... */
		} // (Código createCube anterior - não altere)
		Entity ShapeRenderer::createLine(const math::float3 &p1, const math::float3 &p2,
		                                 const MaterialInstance *materialInstance) {
			/* ... código createLine com material já existente - não altere ... */
		} // (Código createLine com material anterior - não altere)
		Entity ShapeRenderer::createLineList(const std::vector<math::float3> &points,
		                                     const MaterialInstance *materialInstance) {
			/* ... código createLineList com material já existente - não altere ... */
		} // (Código createLineList com material anterior - não altere)


		// --- NOVOS MÉTODOS createLine e createLineList SEM materialInstance (para debug lines) ---

		Entity ShapeRenderer::createLine(const math::float3 &p1, const math::float3 &p2) {
			if (!defaultLineMaterial_) {
				SDL_Log("AVISO: Material padrão para linhas não inicializado. Impossível criar linha de debug.");
				return EntityManager::get().create(); // Retorna entidade vazia se material padrão não existir
			}
			return createLine(p1, p2, defaultLineMaterial_); // Chama a versão com material, usando o material padrão
		}


		Entity ShapeRenderer::createLineList(const std::vector<math::float3> &points) {
			if (!defaultLineMaterial_) {
				SDL_Log(
					"AVISO: Material padrão para linhas não inicializado. Impossível criar lista de linhas de debug.");
				return EntityManager::get().create(); // Retorna entidade vazia se material padrão não existir
			}
			return createLineList(points, defaultLineMaterial_);
			// Chama a versão com material, usando o material padrão
		}
	} // namespace filamentutils
} // namespace utils
