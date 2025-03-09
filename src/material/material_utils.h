#ifndef MATERIAL_UTILS_H
#define MATERIAL_UTILS_H

#include <filament/Engine.h>
#include <filament/Material.h>

namespace utils {
	namespace filamentutils {

		class MaterialUtils {
		public:
			MaterialUtils(filament::Engine* engine);
			~MaterialUtils();

			filament::Material* createMaterial(const char* filamatFilePath);

		private:
			filament::Engine* engine_;
		};

	} // namespace filamentutils
} // namespace utils

#endif // MATERIAL_UTILS_H