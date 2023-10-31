#include "App.hpp"

#include "frameInfo.hpp"
#include "PeripheralInputDevice.hpp"
#include "WindowTypeSpecificInfo.hpp"
#include "callbackFunctions.hpp"

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

#include "../ObjectRenderingCashing.hpp"
#include "../helpers/simulationObjects.hpp"

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
    camera.reset();

    gameObjects3d.clear();
    gameObjects2d.clear();
    staticTexts.clear();
    varyingldsStaticTextRefs.clear();
    varyinglds.clear();

    renderer.reset();
    window.reset();
    device.reset();

    if(typeSpecificInfo != nullptr)
        delete typeSpecificInfo;
}

Vulkan::Vulkan() 
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("Main", WindowType::Menu), loadMainWindow);
    Vulkan::addWindow(WindowInfo::createWindowInfo("Freecam", WindowType::FreeCam), loadFreeCamWindow);
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
    SimulationTimeCash cash = objectLists::objectCash.getsimulationTimeCash(timeObjects::realCurrentTime);
    for (auto it = _windows.begin(); it != _windows.end();) {
        glfwPollEvents();
        if (it->second->window->shouldClose()) {
            it = _windows.erase(it);
            continue;
        }

        for (auto& [key, object] : it->second->gameObjects3d) {
            object.transform = cash.objects[key];
        }

        for (auto& [key, text] : it->second->varyinglds) {
            text.update();
            it->second->varyingldsStaticTextRefs[text.getId()] = &text.staticText();
        }

        auto newTime = std::chrono::high_resolution_clock::now();
        float deltaTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - it->second->currentTime).count();
        it->second->currentTime = newTime;

        if (!_pause) {
            _mouse.rotate(*it->second->window, it->second->camera->rotation);
            _keyboard.rotate(it->second->window->getGLFWwindow(), deltaTime, it->second->camera->rotation);
            _keyboard.move(it->second->window->getGLFWwindow(), deltaTime, it->second->camera->translation, it->second->camera->rotation);
        }
        it->second->camera->setViewYXZ(toVec3(it->second->camera->translation), it->second->camera->rotation);

        double aspect = it->second->renderer->getAspectRatio();
        it->second->camera->setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        if (auto commandBuffer = it->second->renderer->beginFrame()) {
            int frameIndex = it->second->renderer->getFrameIndex();
            FrameInfo frameInfo{
                frameIndex,
                deltaTime,
                commandBuffer,
                *it->second->camera,
                it->second->globalDescriptorSet[frameIndex],
                it->second->gameObjects2d,
                it->second->gameObjects3d,
                it->second->staticTexts,
                it->second->varyingldsStaticTextRefs
            };

            // update
            GlobalUbo ubo{};
            ubo.projection = it->second->camera->getProjection();
            ubo.view = it->second->camera->getView();
            ubo.inverseView = it->second->camera->getInverseView();
            ubo.resolution = glm::vec4{ it->second->window->getExtent().width / (double)std::max(it->second->window->getExtent().width, it->second->window->getExtent().height),
                it->second->window->getExtent().height / (double)std::max(it->second->window->getExtent().width, it->second->window->getExtent().height),0,0};
            it->second->uboBuffer[frameIndex]->writeToBuffer(&ubo);
            it->second->uboBuffer[frameIndex]->flush();

            // render
            it->second->renderer->beginSwapChainRenderPass(commandBuffer);

            // order here matters
            it->second->textRenderSystem->renderText(frameInfo);
            it->second->renderSystem2D->renderGameObjects(frameInfo);
            it->second->renderSystem3D->renderGameObjects(frameInfo);
            it->second->pointLightSystem->render(frameInfo);

            it->second->renderer->endSwapChainRenderPass(commandBuffer);
            it->second->renderer->endFrame();
        }
        it++;
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


void Vulkan::keyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;
    if(key == GLFW_KEY_ESCAPE)
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

    if (key == GLFW_KEY_V) {
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
        WindowInfo* parentWindowInfo;
        for (auto& windowInfo : _windows)
            if (windowInfo.second->window->getGLFWwindow() == window) {
                parentWindowInfo = windowInfo.second.get();
                break;
            }
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
                if (obj.isClicked(mousePos, res, *parentWindowInfo))
                    return;
            }
            return;
        }
    }
}