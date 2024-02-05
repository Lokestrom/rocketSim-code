#include "SwapChain.hpp"

// std
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

SwapChain::SwapChain(Device& deviceRef, vk::Extent2D extent)
    : _device{ deviceRef }, _windowExtent{ extent } {
    init();
}

SwapChain::SwapChain(
    Device& deviceRef, vk::Extent2D extent, std::shared_ptr<SwapChain> previous)
    : _device{ deviceRef }, _windowExtent{ extent }, _oldSwapChain{ previous } {
    init();
    _oldSwapChain = nullptr;
}

void SwapChain::init() {
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDepthResources();
    createTextResources();
    createFramebuffers();
    createSyncObjects();
}

SwapChain::~SwapChain() {
    for (auto& imageView : _swapChainImageViews) {
        _device.device().destroyImageView(imageView);
    }
    _swapChainImageViews.clear();

    if (_swapChain != NULL) {
        _device.device().destroySwapchainKHR(_swapChain);
        _swapChain = nullptr;
    }

    for (int i = 0; i < _depthImages.size(); i++) {
        _device.device().destroyImageView(_depthImageViews[i]);
        _device.device().destroyImage(_depthImages[i]);
        _device.device().freeMemory(_depthImageMemorys[i]);
    }

    for (int i = 0; i < _textImages.size(); i++) {
        _device.device().destroyImageView(_textImageViews[i]);
        _device.device().destroyImage(_textImages[i]);
        _device.device().freeMemory(_textImageMemorys[i]);
    }

    for (auto& framebuffer : _swapChainFramebuffers) {
        _device.device().destroyFramebuffer(framebuffer);
    }

    _device.device().destroyRenderPass(_renderPass);

    // cleanup synchronization objects
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        _device.device().destroySemaphore(_renderFinishedSemaphores[i]);
        _device.device().destroySemaphore(_imageAvailableSemaphores[i]);
        _device.device().destroyFence(_inFlightFences[i]);
    }
}

vk::Result SwapChain::acquireNextImage(uint32_t* imageIndex) {
    _device.device().waitForFences(1, &_inFlightFences[_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

    vk::Result result = _device.device().acquireNextImageKHR(
        _swapChain,
        std::numeric_limits<uint64_t>::max(),
        _imageAvailableSemaphores[_currentFrame],  // must be a not signaled semaphore
        nullptr,
        imageIndex);

    return result;
}

vk::Result SwapChain::submitCommandBuffers(const vk::CommandBuffer* buffers, uint32_t* imageIndex) {
    if (_imagesInFlight[*imageIndex] != NULL) {
        _device.device().waitForFences(1, &_imagesInFlight[*imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());
    }
    _imagesInFlight[*imageIndex] = _inFlightFences[_currentFrame];

    vk::SubmitInfo submitInfo = {};
    submitInfo.sType = vk::StructureType::eSubmitInfo;

    vk::Semaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = buffers;

    vk::Semaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    _device.device().resetFences(1, &_inFlightFences[_currentFrame]);
    if (_device.graphicsQueue().submit(1, &submitInfo, _inFlightFences[_currentFrame]) != vk::Result::eSuccess) {
         std::runtime_error("failed to submit draw command buffer!");
    }

    vk::PresentInfoKHR presentInfo = {};
    presentInfo.sType = vk::StructureType::ePresentInfoKHR;
   
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = { _swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = imageIndex;

    auto result = _device.presentQueue().presentKHR(&presentInfo);

    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    return result;
}

void SwapChain::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = _device.getSwapChainSupport();

    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
    createInfo.surface = _device.surface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    QueueFamilyIndices indices = _device.findPhysicalQueueFamilies();
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.queueFamilyIndexCount = 0;      // Optional
        createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = _oldSwapChain == nullptr ? VK_NULL_HANDLE 
                                                        : _oldSwapChain->_swapChain;

    if (_device.device().createSwapchainKHR(&createInfo, nullptr, &_swapChain) != vk::Result::eSuccess) {
         std::runtime_error("failed to create swap chain!");
    }
    

    // we only specified a minimum number of images in the swap chain, so the implementation is
    // allowed to create a swap chain with more. That's why we'll first query the final number of
    // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
    // retrieve the handles.
    _swapChainImages = _device.device().getSwapchainImagesKHR(_swapChain);

    _swapChainImageFormat = surfaceFormat.format;
    _swapChainExtent = extent;
}

void SwapChain::createImageViews() {
    _swapChainImageViews.resize(_swapChainImages.size());
    for (size_t i = 0; i < _swapChainImages.size(); i++) {
        vk::ImageViewCreateInfo viewInfo{};
        viewInfo.sType = vk::StructureType::eImageViewCreateInfo;
        viewInfo.image = _swapChainImages[i];
        viewInfo.viewType = vk::ImageViewType::e2D;
        viewInfo.format = _swapChainImageFormat;
        viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (_device.device().createImageView(&viewInfo, nullptr, &_swapChainImageViews[i]) != vk::Result::eSuccess) {
             std::runtime_error("failed to create texture image view!");
        }
    }
}

void SwapChain::createRenderPass() {
    vk::AttachmentDescription depthAttachment{};
    depthAttachment.format = findDepthFormat();
    depthAttachment.samples = vk::SampleCountFlagBits::e1;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentDescription textAttachment{};
    textAttachment.format = findTextFormat();
    textAttachment.samples = vk::SampleCountFlagBits::e1;
    textAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    textAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    textAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    textAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    textAttachment.initialLayout = vk::ImageLayout::eUndefined;
    textAttachment.finalLayout = vk::ImageLayout::eTransferDstOptimal;

    vk::AttachmentReference textAttachmentRef{};
    textAttachmentRef.attachment = 2;
    textAttachmentRef.layout = vk::ImageLayout::eTransferDstOptimal;

    vk::AttachmentDescription colorAttachment = {};
    colorAttachment.format = getSwapChainImageFormat();
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    std::array<vk::AttachmentReference, 2> colorAttachments = { colorAttachmentRef, textAttachmentRef };

    vk::SubpassDescription subpass = {};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());
    subpass.pColorAttachments = colorAttachments.data();
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    vk::SubpassDependency dependency = {};
    dependency.dstSubpass = 0;
    dependency.dstAccessMask =
        vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    dependency.dstStageMask =
        vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcAccessMask = vk::AccessFlagBits::eNone;
    dependency.srcStageMask =
        vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;

    std::array<vk::AttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, textAttachment };
    vk::RenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (_device.device().createRenderPass(&renderPassInfo, nullptr, &_renderPass) != vk::Result::eSuccess) {
         std::runtime_error("failed to create render pass!");
    }
}

void SwapChain::createFramebuffers() {
    _swapChainFramebuffers.resize(imageCount());
    for (size_t i = 0; i < imageCount(); i++) {
        std::array<vk::ImageView, 3> attachments = { _swapChainImageViews[i], _depthImageViews[i], _textImageViews[i] };

        vk::Extent2D swapChainExtent = getSwapChainExtent();
        vk::FramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.renderPass = _renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (_device.device().createFramebuffer(&framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != vk::Result::eSuccess) {
             std::runtime_error("failed to create framebuffer!");
        }
    }
}

void SwapChain::createDepthResources() {
    vk::Format depthFormat = findDepthFormat();
    _swapChainDepthFormat = depthFormat;
    vk::Extent2D swapChainExtent = getSwapChainExtent();

    _depthImages.resize(imageCount());
    _depthImageMemorys.resize(imageCount());
    _depthImageViews.resize(imageCount());

    for (int i = 0; i < _depthImages.size(); i++) {
        vk::ImageCreateInfo imageInfo{};
        imageInfo.sType = vk::StructureType::eImageCreateInfo;
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = swapChainExtent.width;
        imageInfo.extent.height = swapChainExtent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = depthFormat;
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;
        imageInfo.flags = vk::ImageCreateFlagBits();

        _device.createImageWithInfo(
            imageInfo,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            _depthImages[i],
            _depthImageMemorys[i]);

        vk::ImageViewCreateInfo viewInfo{};
        viewInfo.sType = vk::StructureType::eImageViewCreateInfo;
        viewInfo.image = _depthImages[i];
        viewInfo.viewType = vk::ImageViewType::e2D;
        viewInfo.format = depthFormat;
        viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (_device.device().createImageView(&viewInfo, nullptr, &_depthImageViews[i]) != vk::Result::eSuccess) {
             std::runtime_error("failed to create texture image view!");
        }
        
    }
}

void SwapChain::createTextResources()
{
    vk::Format textFormat = findTextFormat();
    _swapChainTextFormat = textFormat;
    vk::Extent2D swapChainExtent = getSwapChainExtent();

    _textImages.resize(imageCount());
    _textImageMemorys.resize(imageCount());
    _textImageViews.resize(imageCount());

    for (int i = 0; i < _textImages.size(); i++) {
        vk::ImageCreateInfo imageInfo{};
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = swapChainExtent.width;
        imageInfo.extent.height = swapChainExtent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = textFormat;
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;
        imageInfo.flags = vk::ImageCreateFlagBits();

        _device.createImageWithInfo(
            imageInfo,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            _textImages[i],
            _textImageMemorys[i]);

        vk::ImageViewCreateInfo viewInfo{};
        viewInfo.sType = vk::StructureType::eImageViewCreateInfo;
        viewInfo.image = _textImages[i];
        viewInfo.viewType = vk::ImageViewType::e2D;
        viewInfo.format = textFormat;
        viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (_device.device().createImageView(&viewInfo, nullptr, &_textImageViews[i]) != vk::Result::eSuccess) {
             std::runtime_error("failed to create texture image view!");
        }

    }
}

void SwapChain::createSyncObjects() {
    _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    _imagesInFlight.resize(imageCount(), VK_NULL_HANDLE);

    vk::SemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = vk::StructureType::eSemaphoreCreateInfo;

    vk::FenceCreateInfo fenceInfo = {};
    fenceInfo.sType = vk::StructureType::eFenceCreateInfo;
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (_device.device().createSemaphore(&semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) !=
            vk::Result::eSuccess ||
            _device.device().createSemaphore(&semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) !=
            vk::Result::eSuccess ||
            _device.device().createFence(&fenceInfo, nullptr, &_inFlightFences[i]) != vk::Result::eSuccess) {
             std::runtime_error("failed to create synchronization objects for a frame!");
        }
        
    }
}

vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
            availableFormat.colorSpace == vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

vk::PresentModeKHR SwapChain::chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            std::cout << "Present mode: Mailbox" << std::endl;
            return availablePresentMode;
        }
    }

    std::cout << "Present mode: V-Sync" << std::endl;
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        vk::Extent2D actualExtent = _windowExtent;
        actualExtent.width = std::max(
            capabilities.minImageExtent.width,
            std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(
            capabilities.minImageExtent.height,
            std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

vk::Format SwapChain::findDepthFormat() {
    return _device.findSupportedFormat(
        { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

vk::Format SwapChain::findTextFormat() {
    return _device.findSupportedFormat(
        { vk::Format::eR8Unorm },
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eColorAttachment | vk::FormatFeatureFlagBits::eTransferDst);
}
