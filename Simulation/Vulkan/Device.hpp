#pragma once

#include "Window.hpp"

// std lib headers
#include <string>
#include <vector>
#include <optional>

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

class Device
{
public:
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    Device(Window& window);
    ~Device();

    // Not copyable or movable
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;
    Device(Device&&) = delete;
    Device& operator=(Device&&) = delete;

    vk::CommandPool getCommandPool() { return _commandPool; }
    vk::Device device() { return _logicalDevice; }
    vk::SurfaceKHR surface() { return _surface; }
    vk::Queue graphicsQueue() { return _graphicsQueue; }
    vk::Queue presentQueue() { return _presentQueue; }

    SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(_physicalDevice); }
    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(_physicalDevice); }
    vk::Format findSupportedFormat(
        const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);

    // Buffer Helper Functions
    void createBuffer(
        vk::DeviceSize size,
        vk::BufferUsageFlags usage,
        vk::MemoryPropertyFlags properties,
        vk::Buffer& buffer,
        vk::DeviceMemory& bufferMemory);
    vk::CommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(vk::CommandBuffer commandBuffer);
    void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);
    void copyBufferToImage(
        vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height, uint32_t layerCount);

    void createImageWithInfo(
        const vk::ImageCreateInfo& imageInfo,
        vk::MemoryPropertyFlags properties,
        vk::Image& image,
        vk::DeviceMemory& imageMemory);

    vk::PhysicalDeviceProperties properties;

private:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();

    // helper functions
    bool isDeviceSuitable(vk::PhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);
    void hasGflwRequiredInstanceExtensions();
    bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device);

private:
    vk::Instance _instance;
    vk::DebugUtilsMessengerEXT _debugMessenger;
    vk::DispatchLoaderDynamic _dldi;
    vk::PhysicalDevice _physicalDevice = VK_NULL_HANDLE;
    Window& _window;
    vk::CommandPool _commandPool;

    vk::Device _logicalDevice;
    vk::SurfaceKHR _surface;
    vk::Queue _graphicsQueue;
    vk::Queue _presentQueue;

    const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

