#include "App.hpp"

#include "frameInfo.hpp"
#include "PeripheralInputDevice.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <iostream>

glm::vec4 toVec4(Vector3 v, double r);
Vector3 toVector3(glm::vec3 v);
glm::vec3 toVec3(Vector3 v);

std::unordered_map<unsigned int, std::unique_ptr<WindowInfo>> Vulkan::_windows;
Keyboard Vulkan::_keyboard;
Mouse Vulkan::_mouse;
bool Vulkan::_pause = false;

WindowInfo::WindowInfo(unsigned int id, std::string name, WindowType windowType, void* windowTypeSpecificInfo)
    : ID(id), type(windowType), uboBuffer(SwapChain::MAX_FRAMES_IN_FLIGHT), globalDescriptorSet(SwapChain::MAX_FRAMES_IN_FLIGHT), typeSpecificInfo(windowTypeSpecificInfo) {
    window = std::make_unique<Window>(Vulkan::WIDTH, Vulkan::HEIGHT, name);
    device = std::make_unique<Device>(*window);
    renderer = std::make_unique<Renderer>(*window, *device);

    globalPool = (
        DescriptorPool::Builder(*device)
        .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(vk::DescriptorType::eUniformBuffer, SwapChain::MAX_FRAMES_IN_FLIGHT)
        .build());

    for (int i = 0; i < uboBuffer.size(); i++) {
        uboBuffer[i] = (std::make_unique<Buffer>(
            *device,
            sizeof(GlobalUbo),
            1,
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible));
        uboBuffer[i]->map();
    }
    auto globalSetLayout =
        DescriptorSetLayout::Builder(*device)
        .addBinding(0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eAllGraphics)
        .build();
    for (int i = 0; i < globalDescriptorSet.size(); i++) {
        globalDescriptorSet[i] = (vk::DescriptorSet());
        auto bufferInfo = uboBuffer[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSet[i]);
    }
    renderSystem3D = (std::make_unique<RenderSystem3D>(
        *device,
        renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()));
    renderSystem2D = (std::make_unique<RenderSystem2D>(
        *device,
        renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()));
    pointLightSystem = (std::make_unique<PointLightSystem>(
        *device,
        renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()));
    textRenderSystem = (std::make_unique<TextRenderer>(
        *device,
        renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()));
    camera = std::make_unique<Camera>();

    viewerObject = std::make_unique<GameObject3D>(GameObject3D::createGameObject());
}

WindowInfo::WindowInfo(WindowInfo&& windowInfo) noexcept
{
    ID = windowInfo.ID;
    window.swap(windowInfo.window);
    renderer.swap(windowInfo.renderer);
    device.swap(windowInfo.device);
    globalPool.swap(windowInfo.globalPool);
    gameObjects3d.swap(windowInfo.gameObjects3d);
    gameObjects2d.swap(windowInfo.gameObjects2d);
    staticTexts.swap(windowInfo.staticTexts);
    varyingldsStaticTextRefs.swap(windowInfo.varyingldsStaticTextRefs);
    varyinglds.swap(windowInfo.varyinglds);
    uboBuffer.swap(windowInfo.uboBuffer);
    globalDescriptorSet.swap(windowInfo.globalDescriptorSet);
    renderSystem3D.swap(windowInfo.renderSystem3D);
    renderSystem2D.swap(windowInfo.renderSystem2D);
    pointLightSystem.swap(windowInfo.pointLightSystem);
    textRenderSystem.swap(windowInfo.textRenderSystem);
    camera.swap(windowInfo.camera);
    cameraSetting = windowInfo.cameraSetting;
    viewerObject.swap(windowInfo.viewerObject);
    currentTime = windowInfo.currentTime;
    typeSpecificInfo = windowInfo.typeSpecificInfo;
    windowInfo.typeSpecificInfo = nullptr;
    type = windowInfo.type;
}

WindowInfo::~WindowInfo()
{
    if(device != nullptr)
        device->device().waitIdle();
    globalDescriptorSet.clear();
    uboBuffer.clear();
    globalPool.reset();
    renderSystem3D.reset();
    renderSystem2D.reset();
    pointLightSystem.reset();
    textRenderSystem.reset();
    viewerObject.reset();
    camera.reset();

    gameObjects3d.clear();
    gameObjects2d.clear();
    staticTexts.clear();
    varyingldsStaticTextRefs.clear();
    varyinglds.clear();

    renderer.reset();
    window.reset();
    device.reset();

    delete typeSpecificInfo;
}

void loadShit(WindowInfo& window) {
    std::shared_ptr<Model3D> model = Model3D::createModelFromFile(*window.device, "D:\\code\\codeProjects\\VulkanTest\\models\\colored_cube.obj");
    auto flatVase = GameObject3D::createGameObject();
    flatVase.model = model;
    flatVase.transform.translation = { 0.f, -.1f, 0.f };
    flatVase.transform.scale = { .2f, .2f, .2f };
    window.gameObjects3d.emplace(flatVase.getId(), std::move(flatVase));
}

void shitButtonFunction() {
    Vulkan::addWindow(WindowInfo::createWindowInfo("shit"), loadShit);
    std::cout << "shit button was pressed!\n";
}

void loadWin1(WindowInfo& window)
{
    std::shared_ptr<Model3D> model = Model3D::createModelFromFile(*window.device, "D:\\code\\codeProjects\\VulkanTest\\models\\colored_cube.obj");
    auto flatVase = GameObject3D::createGameObject();
    flatVase.model = model;
    flatVase.transform.translation = { 0.f, -.1f, 0.f };
    flatVase.transform.scale = { .2f, .2f, .2f };
    window.gameObjects3d.emplace(flatVase.getId(), std::move(flatVase));

    model = Model3D::createModelFromFile(*window.device, "D:\\code\\codeProjects\\VulkanTest\\models\\colored_cube.obj");
    auto floor = GameObject3D::createGameObject();
    floor.model = model;
    floor.transform.translation = { 0.f, .5f, 0.f };
    floor.transform.scale = { 3.f, 1.f, 3.f };
    window.gameObjects3d.emplace(floor.getId(), std::move(floor));

    std::vector<glm::vec3> lightColors{
        {1.f, 0.f, .1f},
        {1.f, .1f, 1.f},
        {.1f, .1f, 1.f},
        {.1f, 1.f, .1f},
        {1.f, 1.f, .1f},
        {.1f, 1.f, 1.f},
        {1.f, 1.f, 1.f}
    };

    for (int j = 0; j < lightColors.size(); j++) {
        auto pointLight = GameObject3D::makePointLight(.2f);
        pointLight.color = lightColors[j];
        auto rotateLight = glm::rotate(
            glm::mat4(1.f),
            (j * glm::two_pi<float>()) / lightColors.size(),
            { 0.f, -1.f, 0.f });
        pointLight.transform.translation = toVector3(glm::vec3(rotateLight * glm::vec4(-1.f, -1.0f, .5f, 1.f)));
        window.gameObjects3d.emplace(pointLight.getId(), std::move(pointLight));
    }
    
    auto text = StaticText::createText(*window.device, {-1,0}, {1,1,1,1 }, 0.0001, "H             e\neo");
    window.staticTexts.emplace(text.getId(), std::move(text));
}

Vulkan::Vulkan() 
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("win1"), loadWin1);
}

Vulkan::~Vulkan() {
    glfwTerminate();
}

void Vulkan::startup() {
    for (auto& [key, window] : _windows) {
        glfwSetKeyCallback(window->window->getGLFWwindow(), keyBoardInput);
        glfwSetMouseButtonCallback(window->window->getGLFWwindow(), mouseInput);
        keyBoardInput(window->window->getGLFWwindow(), 0, 0, 0, 0);
    }

    for (auto& [key, window] : _windows)
        window->currentTime = std::chrono::high_resolution_clock::now();
}

bool Vulkan::update() {
    if (_windows.empty())
        return false;
    for (auto& [key, window] : _windows) {
        glfwPollEvents();
        if (window->window->shouldClose()) {
            _windows.erase(key);
            break;
        }
        for (auto& [key, text] : window->varyinglds) {
            text.update();
            window->varyingldsStaticTextRefs[text.getId()] = &text.staticText();
        }

        auto newTime = std::chrono::high_resolution_clock::now();
        float deltaTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - window->currentTime).count();
        window->currentTime = newTime;

        if (!_pause) {
            _mouse.rotate(*window->window, *window->viewerObject);
            _keyboard.rotate(window->window->getGLFWwindow(), deltaTime, *window->viewerObject);
            _keyboard.move(window->window->getGLFWwindow(), deltaTime, *window->viewerObject);
        }
        window->camera->setViewYXZ(toVec3(window->viewerObject->transform.translation), window->viewerObject->transform.rotation);

        double aspect = window->renderer->getAspectRatio();
        window->camera->setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        if (auto commandBuffer = window->renderer->beginFrame()) {
            int frameIndex = window->renderer->getFrameIndex();
            FrameInfo frameInfo{
                frameIndex,
                deltaTime,
                commandBuffer,
                *window->camera,
                window->globalDescriptorSet[frameIndex],
                window->gameObjects2d,
                window->gameObjects3d,
                window->staticTexts,
                window->varyingldsStaticTextRefs
            };

            // update
            GlobalUbo ubo{};
            ubo.projection = window->camera->getProjection();
            ubo.view = window->camera->getView();
            ubo.inverseView = window->camera->getInverseView();
            ubo.resolution = glm::vec4{ window->window->getExtent().width / (float)std::max(window->window->getExtent().width, window->window->getExtent().height), window->window->getExtent().height / (float)std::max(window->window->getExtent().width, window->window->getExtent().height),0,0};
            window->pointLightSystem->update(frameInfo, ubo, _pause);
            window->uboBuffer[frameIndex]->writeToBuffer(&ubo);
            window->uboBuffer[frameIndex]->flush();

            // render
            window->renderer->beginSwapChainRenderPass(commandBuffer);

            // order here matters
            window->textRenderSystem->renderText(frameInfo);
            window->renderSystem2D->renderGameObjects(frameInfo);
            window->renderSystem3D->renderGameObjects(frameInfo);
            window->pointLightSystem->render(frameInfo);

            window->renderer->endSwapChainRenderPass(commandBuffer);
            window->renderer->endFrame();
        }
    }
    return true;
}

void Vulkan::addWindow(WindowInfo window, void (*loadFunction)(WindowInfo&))
{
    unsigned int id = window.ID;
    _windows.emplace(id, std::make_unique<WindowInfo>(std::move(window)));
    glfwSetKeyCallback(_windows[id]->window->getGLFWwindow(), keyBoardInput);
    glfwSetMouseButtonCallback(_windows[id]->window->getGLFWwindow(), mouseInput);
    keyBoardInput(_windows[id]->window->getGLFWwindow(), 0, 0, 0, 0);
    loadFunction(*_windows[id]);
}

/*void App::loadGameObjects() {
    for (int i = 0; i < _windows.size(); i++) {
        _gameObjects.push_back(std::make_unique<GameObject::Map>());
        std::shared_ptr<Model> model = Model::createModelFromFile(*_devices[i], "D:\\code\\codeProjects\\VulkanTest\\models\\colored_cube.obj");
        auto flatVase = GameObject::createGameObject();
        flatVase.model = model;
        flatVase.transform.translation = { 0.f, -.1f, 0.f };
        flatVase.transform.scale = { .2f, .2f, .2f };
        _gameObjects[i]->emplace(flatVase.getId(), std::move(flatVase));

        model = Model::createModelFromFile(*_devices[i], "D:\\code\\codeProjects\\VulkanTest\\models\\quad.obj");
        auto floor = GameObject::createGameObject();
        floor.model = model;
        floor.transform.translation = { 0.f, .5f, 0.f };
        floor.transform.scale = { 3.f, 1.f, 3.f };
        _gameObjects[i]->emplace(floor.getId(), std::move(floor));

        std::vector<glm::vec3> lightColors{
            {1.f, 0.f, .1f},
            {1.f, .1f, 1.f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}
        };

        for (int j = 0; j < lightColors.size(); j++) {
            auto pointLight = GameObject::makePointLight(.2f);
            pointLight.color = lightColors[j];
            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (j * glm::two_pi<float>()) / lightColors.size(),
                { 0.f, -1.f, 0.f });
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.0f, .5f, 1.f));
            _gameObjects[i]->emplace(pointLight.getId(), std::move(pointLight));
        }
    }
}*/


void Vulkan::keyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        _pause = !_pause;
    if (_pause) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        int w = 0, h = 0;
        glfwGetWindowSize(window, &w, &h);
        glfwSetCursorPos(window, w/2, h/2);
    }

    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        for (auto& [key, val] : _windows) {
            if (val->window->getGLFWwindow() != window)
                continue;
            if (val->cameraSetting != CameraSettings::normal) {
                val->cameraSetting = CameraSettings::normal;
                _keyboard.deleteTarget();
                _mouse.deleteTarget();
            }
            else {
                val->cameraSetting = CameraSettings::lookAt;
                _keyboard.setTarget({ 0,-.1,0 });
                _mouse.setTarget({ 0,-.1,0 });
            }
        }
    }
}

void Vulkan::mouseInput(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glm::vec2 mousePos, res;
        double mousex, mousey;
        int w = 0, h = 0;
        glfwGetWindowSize(window, &w, &h);
        glfwGetCursorPos(window, &mousex, &mousey);
        mousePos = glm::vec2{ (mousex*2/w)-1, (mousey*2/h)-1 };

        for (auto& [key, val] : _windows) {
            if (val->window->getGLFWwindow() != window)
                continue;
            for (auto& [id, obj] : val->gameObjects2d) {
                if (obj.type != GameObject2DType::button)
                    continue;
                res = glm::vec2{ val->window->getExtent().width / (double)std::max(val->window->getExtent().width, val->window->getExtent().height), val->window->getExtent().height / (double)std::max(val->window->getExtent().width, val->window->getExtent().height) };
                if (obj.isClicked(mousePos, res))
                    return;
            }
            return;
        }
    }
}