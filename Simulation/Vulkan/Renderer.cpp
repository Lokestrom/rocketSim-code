#include "Renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

Renderer::Renderer(Window& window, Device& device)
    : _window{ window }, _device{ device } {
    recreateSwapChain();
    createCommandBuffers();
}

Renderer::~Renderer() { freeCommandBuffers(); }

void Renderer::recreateSwapChain() {
    auto extent = _window.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = _window.getExtent();
        glfwWaitEvents();
    }
    _device.device().waitIdle();

    if (_swapChain == nullptr) {
        _swapChain = std::make_unique<SwapChain>(_device, extent);
    }
    else {
        std::shared_ptr<SwapChain> oldSwapChain = std::move(_swapChain);
        _swapChain = std::make_unique<SwapChain>(_device, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*_swapChain.get())) {
             std::runtime_error("Swap chain image(or depth) format has changed!");
        }
    }
}

void Renderer::createCommandBuffers() {
    _commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = _device.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

    _commandBuffers = _device.device().allocateCommandBuffers(allocInfo);
}

void Renderer::freeCommandBuffers() {
    _device.device().freeCommandBuffers(_device.getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
    _commandBuffers.clear();
}

vk::CommandBuffer Renderer::beginFrame() {
    assert(!_isFrameStarted && "Can't call beginFrame while already in progress");

    auto result = _swapChain->acquireNextImage(&_currentImageIndex);
    if (result == vk::Result::eErrorOutOfDateKHR) {
        recreateSwapChain();
        return nullptr;
    }

    if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
         std::runtime_error("failed to acquire swap chain image!");
    }

    _isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();
    vk::CommandBufferBeginInfo beginInfo{};

    if (commandBuffer.begin(&beginInfo) != vk::Result::eSuccess) {
         std::runtime_error("failed to begin recording command buffer!");
    }
    
    return commandBuffer;
}

void Renderer::endFrame() {
    assert(_isFrameStarted && "Can't call endFrame while frame is not in progress");
    auto commandBuffer = getCurrentCommandBuffer();
    commandBuffer.end();

    auto result = _swapChain->submitCommandBuffers(&commandBuffer, &_currentImageIndex);
    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR ||
        _window.wasWindowResized()) {
        _window.resetWindowResizedFlag();
        recreateSwapChain();
    }
    else if (result != vk::Result::eSuccess) {
         std::runtime_error("failed to present swap chain image!");
    }

    _isFrameStarted = false;
    _currentFrameIndex = (_currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(vk::CommandBuffer commandBuffer) {
    assert(_isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(
        commandBuffer == getCurrentCommandBuffer() &&
        "Can't begin render pass on command buffer from a different frame");

    vk::RenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = vk::StructureType::eRenderPassBeginInfo;
    renderPassInfo.renderPass = _swapChain->getRenderPass();
    renderPassInfo.framebuffer = _swapChain->getFrameBuffer(_currentImageIndex);

    renderPassInfo.renderArea.offset = vk::Offset2D{ 0, 0 };
    renderPassInfo.renderArea.extent = _swapChain->getSwapChainExtent();

    std::array<vk::ClearValue, 3> clearValues{};
    clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
    clearValues[1].depthStencil = vk::ClearDepthStencilValue{ 1.0f, 0 };
    clearValues[2].color = { 1,1,1,1 };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

    vk::Viewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_swapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(_swapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vk::Rect2D scissor{ {0, 0}, _swapChain->getSwapChainExtent() };
    commandBuffer.setViewport(0, 1, &viewport);
    commandBuffer.setScissor(0, 1, &scissor);
}

void Renderer::endSwapChainRenderPass(vk::CommandBuffer commandBuffer) {
    assert(_isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
    assert(
        commandBuffer == getCurrentCommandBuffer() &&
        "Can't end render pass on command buffer from a different frame");
    commandBuffer.endRenderPass();
}