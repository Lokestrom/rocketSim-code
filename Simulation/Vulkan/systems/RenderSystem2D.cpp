#include "RenderSystem2D.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

struct RenderSystem2DPushConstants {
    glm::vec4 position{};
    glm::vec2 scale{};
};

RenderSystem2D::RenderSystem2D(
    Device& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout)
    : _device{ device } {
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

RenderSystem2D::~RenderSystem2D() {
    _device.device().destroyPipelineLayout(_pipelineLayout);
}

void RenderSystem2D::createPipelineLayout(vk::DescriptorSetLayout globalSetLayout) {
    vk::PushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(RenderSystem2DPushConstants);

    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    _pipelineLayout = _device.device().createPipelineLayout(pipelineLayoutInfo);
}

void RenderSystem2D::createPipeline(vk::RenderPass renderPass) {

    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo2D(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = _pipelineLayout;
    _pipeline = std::make_unique<Pipeline>(
        _device,
        "D:/code/codeProjects/VulkanTest/shaders/shader2d.vert.spv",
        "D:/code/codeProjects/VulkanTest/shaders/shader2d.frag.spv",
        pipelineConfig);
}

void RenderSystem2D::renderGameObjects(FrameInfo& frameInfo) {
    _pipeline->bind(frameInfo.commandBuffer);

    frameInfo.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (auto& kv : frameInfo.gameObjects2D) {
        auto& obj = kv.second;
        if (obj.model == nullptr) continue;
        RenderSystem2DPushConstants push{};
        push.position = { obj.transform.translation, obj.transform.rotation};
        push.scale = obj.transform.scale;

        frameInfo.commandBuffer.pushConstants(_pipelineLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(RenderSystem2DPushConstants), &push);
        obj.model->bind(frameInfo.commandBuffer);
        obj.model->draw(frameInfo.commandBuffer);
    }
}