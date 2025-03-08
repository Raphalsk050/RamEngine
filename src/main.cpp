#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL_syswm.h>
#include <vulkan/vulkan.h>

// Headers do Filament
#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/SwapChain.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Skybox.h>
#include <filament/Camera.h>
#include <utils/EntityManager.h>
#include <math/vec4.h>
#include <math/vec3.h>

#undef Success
#include <filament/LightManager.h>
#include <filament/RenderableManager.h>

// change this to false if you want to change from Vulkan to OpenGL
#define USE_VULKAN true

using namespace filament;
using namespace utils;

void* GetNativeWindow(SDL_Window* sdlWindow) {
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(sdlWindow, &wmi);
    Window win = wmi.info.x11.window;
    return reinterpret_cast<void*>(win);
}

int main(int argc, char** argv) {
    setenv("VK_INSTANCE_LAYERS", "VK_LAYER_KHRONOS_validation", 1);
    setenv("SDL_VIDEODRIVER", "x11", 1);
    setenv("FILAMENT_DEBUG", "1", 1);
    setenv("FILAMENT_LOG_LEVEL", "verbose", 1);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        SDL_Log("Error to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Filament Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_Log("Failed to create SDL window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // create filament engine
    Engine* engine;
    if (USE_VULKAN) {
        engine = Engine::create(backend::Backend::VULKAN);
    } else {
        engine = Engine::create(backend::Backend::OPENGL);
    }

    // create swap chain
    SwapChain* swapChain = engine->createSwapChain(GetNativeWindow(window));

    Renderer* renderer = engine->createRenderer();
    renderer->setClearOptions({.clearColor = {0, 0, 0, 0}, .clear = true});

    // create Scene and View
    Scene* scene = engine->createScene();
    View* view = engine->createView();
    view->setScene(scene);

    // create camera
    Camera* camera = engine->createCamera(EntityManager::get().create());
    camera->setProjection(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    camera->lookAt({0, 0, 10}, {0, 0, 0}, {0, 1, 0});
    view->setCamera(camera);


    // create Skybox
    Skybox* skybox = Skybox::Builder()
        .color(math::float4(0.1f, 0.1f, 0.4f, 1.0f))
        .build(*engine);
    scene->setSkybox(skybox);

    // create directional light
    Entity light = EntityManager::get().create();
    LightManager::Builder(LightManager::Type::DIRECTIONAL)
        .color(math::float3(1.0f, 1.0f, 1.0f))
        .intensity(100000.0f)
        .direction(math::float3(0.0f, -1.0f, -1.0f))
        .build(*engine, light);setenv("VK_INSTANCE_LAYERS", "VK_LAYER_KHRONOS_validation", 1);
    scene->addEntity(light);


    SDL_Log("initializing render loop");

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_Log("Rendering frame");

        if (!renderer->beginFrame(swapChain)) {
            SDL_Log("ERROR: beginFrame() failed!");
        } else {
            renderer->render(view);
            renderer->endFrame();
        }

        SDL_Delay(16);
    }

    SDL_Log("🛑 Quiting application");

    engine->destroy(skybox);
    engine->destroy(view);
    engine->destroy(scene);
    engine->destroy(renderer);
    engine->destroy(swapChain);
    Engine::destroy(&engine);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
