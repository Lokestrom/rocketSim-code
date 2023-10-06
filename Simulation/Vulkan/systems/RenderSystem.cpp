#include "RenderSystem.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>
#include <iostream>

struct PushConstantData3D {
    glm::mat4 modelMatrix{ 1.f };
    glm::mat4 normalMatrix{ 1.f };
};

RenderSystem3D::RenderSystem3D(
    Device& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout)
    : _device{ device } {
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

RenderSystem3D::~RenderSystem3D() {
    _device.device().destroyPipelineLayout(_pipelineLayout);
}

void RenderSystem3D::createPipelineLayout(vk::DescriptorSetLayout globalSetLayout) {
    vk::PushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PushConstantData3D);

    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    try {
        _pipelineLayout = _device.device().createPipelineLayout(pipelineLayoutInfo);
    }
    catch (vk::SystemError& err) {
        std::cout << "failed to create pipeline layout";
    }
}

void RenderSystem3D::createPipeline(vk::RenderPass renderPass) {
    assert(_pipelineLayout != NULL && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo3D(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = _pipelineLayout;
    _pipeline = std::make_unique<Pipeline>(
        _device,
        "D:\\code\\codeProjects\\VulkanTest\\shaders\\simple_shader.vert.spv",
        "D:\\code\\codeProjects\\VulkanTest\\shaders\\simple_shader.frag.spv",
        pipelineConfig);
}

void RenderSystem3D::renderGameObjects(FrameInfo& frameInfo) {
    _pipeline->bind(frameInfo.commandBuffer);

    frameInfo.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (auto& kv : frameInfo.gameObjects3D) {
        auto& obj = kv.second;
        if (obj.model == nullptr) continue;
        PushConstantData3D push{};
        push.modelMatrix = obj.transform.mat4();
        push.normalMatrix = obj.transform.normalMatrix();

        frameInfo.commandBuffer.pushConstants(_pipelineLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(PushConstantData3D), &push);
        obj.model->bind(frameInfo.commandBuffer);
        obj.model->draw(frameInfo.commandBuffer);
    }
}