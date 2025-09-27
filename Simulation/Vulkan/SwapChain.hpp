#pragma once

#include "Device.hpp"

// std lib headers
#include <memory>
#include <string>
#include <vector>

class SwapChain
{
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    SwapChain(Device& deviceRef, vk::Extent2D windowExtent);
    SwapChain(
        Device& deviceRef, vk::Extent2D windowExtent, std::shared_ptr<SwapChain> previous);

    ~SwapChain();

    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;

    /*getters*/
    vk::Framebuffer getFrameBuffer(int index) { return _swapChainFramebuffers[index]; }
    vk::RenderPass getRenderPass() { return _renderPass; }
    vk::ImageView getImageView(int index) { return _swapChainImageViews[index]; }
    size_t imageCount() { return _swapChainImages.size(); }
    vk::Format getSwapChainImageFormat() { return _swapChainImageFormat; }
    vk::Extent2D getSwapChainExtent() { return _swapChainExtent; }
    uint32_t width() { return _swapChainExtent.width; }
    uint32_t height() { return _swapChainExtent.height; }

    double extentAspectRatio() {
        return static_cast<double>(_swapChainExtent.width) / static_cast<double>(_swapChainExtent.height);
    }
    vk::Format findDepthFormat();

    vk::Result acquireNextImage(uint32_t* imageIndex);
    vk::Result submitCommandBuffers(const vk::CommandBuffer* buffers, uint32_t* imageIndex);

    bool compareSwapFormats(const SwapChain& swapChain) const {
        return swapChain._swapChainDepthFormat == _swapChainDepthFormat &&
            swapChain._swapChainImageFormat == _swapChainImageFormat;
    }

private:
    void init();
    void createSwapChain();
    void createImageViews();
    void createDepthResources();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();

    // Helper functions
    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    vk::PresentModeKHR chooseSwapPresentMode(
        const std::vector<vk::PresentModeKHR>& availablePresentModes);
    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const;

private:
    vk::Format _swapChainImageFormat;
    vk::Format _swapChainDepthFormat;
    vk::Format _swapChainTextFormat;
    vk::Extent2D _swapChainExtent;

    std::vector<vk::Framebuffer> _swapChainFramebuffers;
    vk::RenderPass _renderPass;

    std::vector<vk::Image> _swapChainImages;
    std::vector<vk::ImageView> _swapChainImageViews;
    std::vector<vk::Image> _depthImages;
    std::vector<vk::DeviceMemory> _depthImageMemorys;
    std::vector<vk::ImageView> _depthImageViews;
    std::vector<vk::Image> _textImages;
    std::vector<vk::DeviceMemory> _textImageMemorys;
    std::vector<vk::ImageView> _textImageViews;

    Device& _device;
    vk::Extent2D _windowExtent;

    vk::SwapchainKHR _swapChain;
    std::shared_ptr<SwapChain> _oldSwapChain;

    std::vector<vk::Semaphore> _imageAvailableSemaphores;
    std::vector<vk::Semaphore> _renderFinishedSemaphores;
    std::vector<vk::Fence> _inFlightFences;
    std::vector<vk::Fence> _imagesInFlight;
    size_t _currentFrame = 0;
};

