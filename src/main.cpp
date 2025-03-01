#include "engine/core/engine.h"

int main() {
	Engine* engine = new Engine();
	engine->init();
	engine->run();
	return 0;
}