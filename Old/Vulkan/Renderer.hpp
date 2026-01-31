#pragma once

#include "Device.hpp"
#include "SwapChain.hpp"
#include "Window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

class Renderer
{
public:
    Renderer(Window& window, Device& device);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    /*getters*/
    vk::RenderPass getSwapChainRenderPass() const { return _swapChain->getRenderPass(); }
    double getAspectRatio() const { return _swapChain->extentAspectRatio(); }
    bool isFrameInProgress() const { return _isFrameStarted; }
    vk::CommandBuffer getCurrentCommandBuffer() const {
        assert(_isFrameStarted && "Cannot get command buffer when frame not in progress");
        return _commandBuffers[_currentFrameIndex];
    }
    int getFrameIndex() const {
        assert(_isFrameStarted && "Cannot get frame index when frame not in progress");
        return _currentFrameIndex;
    }

    vk::CommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(vk::CommandBuffer commandBuffer);
    void endSwapChainRenderPass(vk::CommandBuffer commandBuffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

private:
    Window& _window;
    Device& _device;
    std::unique_ptr<SwapChain> _swapChain;
    std::vector<vk::CommandBuffer> _commandBuffers;

    uint32_t _currentImageIndex;
    int _currentFrameIndex{ 0 };
    bool _isFrameStarted{ false };
};

