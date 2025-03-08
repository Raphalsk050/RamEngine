#include "material_utils.h",
#include <filament/Engine.h>
#include <filament/Material.h>
#include <fstream>
#include <SDL2/SDL.h> // Para SDL_Log (pode remover se tiver outra forma de log)

using namespace filament;

namespace utils {
	namespace filamentutils {
		MaterialUtils::MaterialUtils(Engine *engine) : engine_(engine) {
			// Construtor vazio por enquanto
		}

		MaterialUtils::~MaterialUtils() {
			// Destrutor vazio por enquanto
		}

		Material *MaterialUtils::createMaterial(const char *filamatFilePath) {
			std::ifstream materialFileStream(filamatFilePath, std::ios::binary | std::ios::in);
			if (!materialFileStream.is_open()) {
				SDL_Log("ERRO: Falha ao abrir o arquivo .filamat do material: %s", filamatFilePath);
				return nullptr; // Retorna nullptr em caso de erro
			}
			materialFileStream.seekg(0, std::ifstream::end); // Modificado para std::ifstream::end
			size_t materialFileSize = materialFileStream.tellg();
			materialFileStream.seekg(0, std::ifstream::beg); // Modificado para std::ifstream::beg
			char *materialData = new char[materialFileSize];
			materialFileStream.read(materialData, materialFileSize);
			materialFileStream.close();

			Material *material = Material::Builder()
					.build(*engine_);
			// Modificado para usar build() com data e size diretamente

			delete[] materialData; // Liberar memória alocada

			return material;
		}
	} // namespace filamentutils
} // namespace utils
