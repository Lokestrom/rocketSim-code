#include "TextSystem.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

struct TextRendererPushConstants {
    glm::vec4 position;
    float scale{};
};

TextRenderer::TextRenderer(
    Device& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout)
    : _device{ device } {
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

TextRenderer::~TextRenderer() {
    _device.device().destroyPipelineLayout(_pipelineLayout);
}

void TextRenderer::createPipelineLayout(vk::DescriptorSetLayout globalSetLayout) {
    vk::PushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(TextRendererPushConstants);

    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    _pipelineLayout = _device.device().createPipelineLayout(pipelineLayoutInfo);
}

void TextRenderer::createPipeline(vk::RenderPass renderPass) {

    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfoText(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = _pipelineLayout;
    _pipeline = std::make_unique<Pipeline>(
        _device,
        "D:/code/codeProjects/VulkanTest/shaders/shaderText.vert.spv",
        "D:/code/codeProjects/VulkanTest/shaders/shaderText.frag.spv",
        pipelineConfig);
}

void TextRenderer::renderText(FrameInfo& frameInfo) {
    _pipeline->bind(frameInfo.commandBuffer);

    frameInfo.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (auto& kv : frameInfo.staticText) {
        auto& text = kv.second;
        TextRendererPushConstants push{};
        push.position = text.getPos();
        push.scale = text.getScale();

        frameInfo.commandBuffer.pushConstants(_pipelineLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(TextRendererPushConstants), &push);
        text.bind(frameInfo.commandBuffer);
        text.draw(frameInfo.commandBuffer);
    }
    for (auto& kv : frameInfo.varyingText) {
        auto& text = kv.second;
        TextRendererPushConstants push{};
        push.position = text->getPos();
        push.scale = text->getScale();

        frameInfo.commandBuffer.pushConstants(_pipelineLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(TextRendererPushConstants), &push);
        text->bind(frameInfo.commandBuffer);
        text->draw(frameInfo.commandBuffer);
    }
}