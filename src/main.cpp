#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL_syswm.h>
#include <vulkan/vulkan.h>
#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Viewport.h>
#include <filament/TransformManager.h>
#include <filament/SwapChain.h>
#include <backend/BufferDescriptor.h>
#include <filament/View.h>
#include <filament/Skybox.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Scene.h>
#include <filament/Camera.h>
#include <filament/FilamentAPI.h>
#include <utils/EntityManager.h>
#include <utils/Entity.h>
#include <chrono> // Required for std::chrono and sleep_for
#include <thread> // Required for std::this_thread

#include "MeshUtils/shape_renderer.h"

#undef Success
#include <filament/LightManager.h>
#include <filament/RenderableManager.h>

#include "core/base/node.h"

using namespace utils;
using namespace filament;

void* GetNativeWindow(SDL_Window* sdlWindow) {
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(sdlWindow, &wmi);
    Window win = wmi.info.x11.window;
    return reinterpret_cast<void*>(win);
}

int main() {

    int newWidth = 800;
    int newHeight = 600;

    setenv("VK_INSTANCE_LAYERS", "VK_LAYER_KHRONOS_validation", 1);
    setenv("SDL_VIDEODRIVER", "x11", 1);
    setenv("FILAMENT_DEBUG", "1", 1);
    setenv("FILAMENT_LOG_LEVEL", "verbose", 1);

    // Create the engine
    Engine* engine = Engine::create();
    TransformManager& transformManager = engine->getTransformManager();

    // Create a native window (using SDL2 or another windowing library)
    SDL_Window* window = SDL_CreateWindow("Filament Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    // Create a swap chain
    SwapChain* swapChain = engine->createSwapChain(GetNativeWindow(window));

    // Create a renderer
    Renderer* renderer = engine->createRenderer();

    // Create a view
    View* view = engine->createView();

    // renderers a new frame instantly after the swap chain creation
    if (!renderer->beginFrame(swapChain)) {
        SDL_Log("ERROR: beginFrame() falhou no frame inicial (após criação do SwapChain)!");
    } else {
        renderer->render(view);
        renderer->endFrame();
        SDL_Log("INFO: Frame inicial renderizado após criação do SwapChain.");
    }

    // create camera
    Node* camera_node = new Node(engine);
    Camera* camera = engine->createCamera(camera_node->getEntity());
    camera->setProjection(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    camera->lookAt({0, 0, 10}, {0, 0, 0}, {0, 1, 0});

    // Create a scene
    Scene* scene = engine->createScene();

    // create Skybox
    Skybox* skybox = Skybox::Builder()
        .color(math::float4(0.1f, 0.1f, 0.4f, 1.0f))
        .build(*engine);
    scene->setSkybox(skybox);

    // create directional light
    Node* light_node = new Node(engine);

    LightManager::Builder(LightManager::Type::DIRECTIONAL)
        .color(math::float3(1.0f, 1.0f, 1.0f))
        .intensity(100000.0f)
        .direction(math::float3(0.0f, -1.0f, -1.0f))
        .build(*engine, light_node->getEntity());

    auto shape_renderer = filamentutils::ShapeRenderer(engine, scene);
    //Node* node = shape_renderer.createLine(math::float3(0.0f, 0.0f, 13.0f), math::float3(1.0f, 1.0f, 14.0f));

    view->setCamera(camera);
    view->setScene(scene);
    scene->addEntity(light_node->getEntity());

    // Main rendering loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_Log("INFO: Janela redimensionada. Reconstruindo SwapChain e Viewport.");
                newWidth = event.window.data1;
                newHeight = event.window.data2;

                // 1. Destruir SwapChain antigo e criar novo
                engine->destroy(swapChain);
                swapChain = engine->createSwapChain(GetNativeWindow(window));

                // 2. Ajustar o viewport da View para o novo tamanho da janela
                view->setViewport(Viewport(0,0,newWidth,newHeight));

                // 3. Ajustar a proporção da câmera (projection)
                camera->setProjection(45.0f, (float)newWidth / (float)newHeight, 0.1f, 100.0f);

                SDL_Log("INFO: Reconstrução e ajustes completos.");
            }
        }

        SDL_Log("Rendering frame");

        if (!renderer->beginFrame(swapChain)) {
            SDL_Log("ERROR: beginFrame() falhou!");
            // Reconstruir o SwapChain quando beginFrame() falha
            engine->destroy(swapChain); // Destruir o SwapChain antigo
            swapChain = engine->createSwapChain(GetNativeWindow(window)); // Criar um novo
            SDL_Log("INFO: SwapChain reconstruído."); // Log para verificar a reconstrução
        } else {
            view->setViewport(Viewport(0,0,newWidth,newHeight));
            renderer->render(view);
            renderer->endFrame();
        }

        SDL_Delay(16);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ADDED DELAY HERE BEFORE CLEANUP

    // Clean up
    engine->destroy(skybox);
    engine->destroy(scene); // Destruir Scene ANTES de View
    engine->destroy(view);
    engine->destroy(renderer);
    engine->destroy(swapChain);
    EntityManager::get().destroy(light_node->getEntity()); // Destroy the light entity!
    Engine::destroy(&engine); // Engine SEMPRE por último
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}