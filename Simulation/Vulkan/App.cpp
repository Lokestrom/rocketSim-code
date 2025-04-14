#include "App.hpp"

#include "frameInfo.hpp"
#include "PeripheralInputDevice.hpp"
#include "UI.hpp"

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
#include "UI.hpp"
#include "windowFunctions/windowFunctions.hpp"

#include "Camera.hpp"

#include "systems/PointLightSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/RenderSystem2D.hpp"
#include "systems/TextSystem.hpp"

#include "../rocket/Rocket.hpp"
#include "Renderer.hpp"
#include "Device.hpp"
#include "Window.hpp"
#include "Buffer.hpp"
#include "SwapChain.hpp"
#include <string>

glm::vec4 toVec4(Vector3 v, double r);
Vector3 toVector3(glm::vec3 v);
glm::vec3 toVec3(Vector3 v);

WindowInfo::WindowInfo(unsigned int id, std::string name, windows::Type windowType, void* windowTypeSpecificInfo)
    : ID(id), type(windowType), uboBuffer(SwapChain::MAX_FRAMES_IN_FLIGHT), globalDescriptorSet(SwapChain::MAX_FRAMES_IN_FLIGHT),
    typeSpecificInfo((windowTypeSpecificInfo == nullptr)
        ? windows::createInfo(type) 
        : windowTypeSpecificInfo),
    closeWindow(false), background()
{
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
    pointLightSystem = (std::make_unique<PointLightSystem>(
        *device,
        renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()));
    textRenderSystem = (std::make_unique<TextRenderer>(
        *device,
        renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()));
    renderSystem2D = (std::make_unique<RenderSystem2D>(
        *device,
        renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()));
    camera = std::make_unique<Camera>();
}

void WindowInfo::prepereForTypeSwap()
{
    device->device().waitIdle();
    gameObjects3d.clear();
    UIElements.clear();
    staticTexts.clear();
    varyingTexts.clear();
    forum.clear();

    textInputFields.clear();
    buttons.clear();

    background.reset();

    windows::getDeleteInfoFunction(type)(typeSpecificInfo);
}

WindowInfo::WindowInfo(WindowInfo&& windowInfo) noexcept
{
    ID = windowInfo.ID;
    window.swap(windowInfo.window);
    renderer.swap(windowInfo.renderer);
    device.swap(windowInfo.device);
    globalPool.swap(windowInfo.globalPool);
    gameObjects3d.swap(windowInfo.gameObjects3d);
    UIElements.swap(windowInfo.UIElements);
    staticTexts.swap(windowInfo.staticTexts);
    varyingTexts.swap(windowInfo.varyingTexts);
    uboBuffer.swap(windowInfo.uboBuffer);
    globalDescriptorSet.swap(windowInfo.globalDescriptorSet);
    renderSystem3D.swap(windowInfo.renderSystem3D);
    renderSystem2D.swap(windowInfo.renderSystem2D);
    pointLightSystem.swap(windowInfo.pointLightSystem);
    textRenderSystem.swap(windowInfo.textRenderSystem);
    camera.swap(windowInfo.camera);
    currentTime = windowInfo.currentTime;
    typeSpecificInfo = windowInfo.typeSpecificInfo;
    windowInfo.typeSpecificInfo = nullptr;
    type = windowInfo.type;
    closeWindow = windowInfo.closeWindow;
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
    UIElements.clear();
    staticTexts.clear();
    varyingTexts.clear();

    renderer.reset();
    window.reset();
    device.reset();

    windows::getDeleteInfoFunction(type)(typeSpecificInfo);
}

WindowInfo& WindowInfo::getWindowInfo(GLFWwindow* glfwWindow)
{
    for (const auto& i : Vulkan::getWindows()) {
        if (i.second->window->getGLFWwindow() == glfwWindow)
            return *i.second.get();
    }
    Error("no WindowInfo with contains the given glfwWindow", Error::Type::codeFault);
}

Vulkan::Vulkan() 
{
    windows::createWindow(windows::Type::StartMenu);
}

Vulkan::~Vulkan() {
    glfwTerminate();
}

void Vulkan::startup() {
    for (auto& [key, window] : _windows) {
        resetCallback(window->window->getGLFWwindow());
        keyBoardInput(window->window->getGLFWwindow(), 0, 0, 0, 0);
    }

    for (auto& [key, window] : _windows)
        window->currentTime = std::chrono::high_resolution_clock::now();
}

bool Vulkan::update() {
    if (_windows.empty()) [[unlikely]]
        return false;
    if(!_pause)
        _currentSimulationState = objectLists::objectCash.getsimulationTimeCash(timeObjects::realRunTime);
    
    for (auto it = _windows.begin(); it != _windows.end();) {
        glfwPollEvents();
        if (_reset) [[unlikely]] {
            it++;
            continue;
        }
        if (it->second->window->shouldClose() || it->second->closeWindow) [[unlikely]] {
            it = _windows.erase(it);
            continue;
        }

        for (auto& [_, i] : it->second->varyingTexts) {
            i->update();
        }
        for (auto i : it->second->textInputFields)
            i->update(*it->second);

        if(it->second->type == windows::Type::FreeCam)
            for (auto& [id, transform] : _currentSimulationState.objects) {
                it->second->gameObjects3d.at(id).transform = transform;
            }

        auto newTime = std::chrono::high_resolution_clock::now();
        float deltaTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - it->second->currentTime).count();
        timeObjects::fps = 1 / deltaTime;
        it->second->currentTime = newTime;

        it->second->camera->update(*it->second, deltaTime, _keyboard, _mouse, _pause);

        if (auto commandBuffer = it->second->renderer->beginFrame()) {
            int frameIndex = it->second->renderer->getFrameIndex();
            FrameInfo frameInfo{
                frameIndex,
                deltaTime,
                commandBuffer,
                *it->second->camera,
                it->second->globalDescriptorSet[frameIndex],
                it->second->UIElements,
                it->second->gameObjects3d,
                it->second->staticTexts,
            };

            // update
            GlobalUbo ubo{};
            ubo.projection = it->second->camera->getProjection();
            ubo.view = it->second->camera->getView();
            ubo.inverseView = it->second->camera->getInverseView();
            ubo.resolution = glm::vec4{ it->second->window->getExtent().width / (double)std::max(it->second->window->getExtent().width, it->second->window->getExtent().height),
                it->second->window->getExtent().height / (double)std::max(it->second->window->getExtent().width, it->second->window->getExtent().height),0,0};
            it->second->pointLightSystem->update(frameInfo, ubo);
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
            it++;
        }
    }
    _reset = false;
    return true;
}

void Vulkan::addWindow(WindowInfo window, void (*loadFunction)(WindowInfo&))
{
    unsigned int id = window.ID;
    _windows.emplace(id, std::make_unique<WindowInfo>(std::move(window)));
	resetCallback(_windows[id]->window->getGLFWwindow());
    keyBoardInput(_windows[id]->window->getGLFWwindow(), 0, 0, 0, 0);
    loadFunction(*_windows[id]);
}

void Vulkan::resetCallback(GLFWwindow* window)
{
    glfwSetKeyCallback(window, keyBoardInput);
    glfwSetMouseButtonCallback(window, mouseInput);
    glfwSetCharCallback(window, nullptr);
    glfwSetScrollCallback(window, scrollInput);
}


void Vulkan::keyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS) [[likely]]
        return;
    if (key == GLFW_KEY_ESCAPE) {
        _pause = !_pause;
        if (_pause) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            timeObjects::pauseStartTimeEpoch = timeObjects::getTimeSinceEpoch();
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            int w = 0, h = 0;
            glfwGetWindowSize(window, &w, &h);
            glfwSetCursorPos(window, w / 2, h / 2);
            timeObjects::realStartTimeEpoch += timeObjects::getTimeSinceEpoch() - timeObjects::pauseStartTimeEpoch;

        }
    }

    if (key == GLFW_KEY_M) {
        if (_mouse.enabled) {
            _mouse.enabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            _mouse.enabled = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    if (key == GLFW_KEY_V) {
        for (auto& [key, val] : _windows) {
            if (val->window->getGLFWwindow() != window)
                continue;
            switch (val->camera->setting)
            {
            case CameraSettings::normal:
                val->camera->setting = CameraSettings::follow;
                val->camera->followObj = objectLists::rockets[0]->stages()[0]->getID().getID();
                val->camera->transform.translation -= val->gameObjects3d.at(val->camera->followObj.value()).transform.translation;
                break;

            case CameraSettings::follow:
                val->camera->setting = CameraSettings::lookAt;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                val->camera->transform.translation += val->gameObjects3d.at(val->camera->followObj.value()).transform.translation;
                val->camera->followObj = objectLists::rockets[0]->stages()[0]->getID().getID();
                break;

            case CameraSettings::lookAt:
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                val->camera->setting = CameraSettings::normal;
                val->camera->followObj.reset();
                break;

            default:
                break;
            }
        }
    }
}

void Vulkan::mouseInput(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        WindowInfo& parentWindowInfo = WindowInfo::getWindowInfo(window);
        Vector2 mousePos, res;
        double mousex, mousey;
        int w = 0, h = 0;
        glfwGetWindowSize(window, &w, &h);
        glfwGetCursorPos(window, &mousex, &mousey);
        mousePos = Vector2{ (mousex*2/w)-1, (mousey*2/h)-1 };
        res = Vector2{ (ld)parentWindowInfo.window->getExtent().width, (ld)parentWindowInfo.window->getExtent().height };
        res /= (ld)std::max(parentWindowInfo.window->getExtent().width, parentWindowInfo.window->getExtent().height);
        
        for (auto& i : parentWindowInfo.textInputFields) {
            if(i->isClicked(mousePos, res, parentWindowInfo))
                return;
        }
        for (auto& i : parentWindowInfo.buttons) {
            if (i->isClicked(mousePos, res, parentWindowInfo))
                return;
        }
    }
}

void Vulkan::scrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
	_keyboard.changeLookSpeed(yoffset);
}