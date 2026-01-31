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
    glm::vec4 color;  
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

    PipelineConfigInfo hardPipelineConfig{};
    Pipeline::hardPipelineConfigInfoText(hardPipelineConfig);
    hardPipelineConfig.renderPass = renderPass;
    hardPipelineConfig.pipelineLayout = _pipelineLayout;
    hardPipelineConfig.vertFilepath = ".\\Vulkan\\shaders\\text\\hard.vert.spv";
    //hardPipelineConfig.fragFilepath = ".\\Vulkan\\shaders\\text\\hard.frag.spv";
    _hardPipeline = std::make_unique<Pipeline>(_device, hardPipelineConfig);

    PipelineConfigInfo becierPipelineConfig{};
    Pipeline::becierPipelineConfigInfoText(becierPipelineConfig);
    becierPipelineConfig.renderPass = renderPass;
    becierPipelineConfig.pipelineLayout = _pipelineLayout;
    becierPipelineConfig.vertFilepath = ".\\Vulkan\\shaders\\text\\becier.vert.spv";
    becierPipelineConfig.fragFilepath = ".\\Vulkan\\shaders\\text\\becier.frag.spv";
    _becierPipeline = std::make_unique<Pipeline>(_device, becierPipelineConfig);

    PipelineConfigInfo boxPipelineConfig{};
    Pipeline::boxPipelineConfigInfoText(boxPipelineConfig);
    boxPipelineConfig.renderPass = renderPass;
    boxPipelineConfig.pipelineLayout = _pipelineLayout;
    boxPipelineConfig.vertFilepath = ".\\Vulkan\\shaders\\text\\box.vert.spv";
    boxPipelineConfig.fragFilepath = ".\\Vulkan\\shaders\\text\\box.frag.spv";
    _boxPipeline = std::make_unique<Pipeline>(_device, boxPipelineConfig);
}

void TextRenderer::renderText(FrameInfo& frameInfo) {

    frameInfo.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (auto& kv : frameInfo.staticText) {
        auto& text = *kv.second;
        if (text.empty())
            continue;
        TextRendererPushConstants push{};
        push.position = glm::vec4(text.getPos().x, text.getPos().y, 0, text.getScale());
        push.color = glm::vec4(text.getColor());

        _hardPipeline->bind(frameInfo.commandBuffer);
        frameInfo.commandBuffer.pushConstants(_pipelineLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(TextRendererPushConstants), &push);
        text.bindHard(frameInfo.commandBuffer);
        text.drawHard(frameInfo.commandBuffer);

        if (text.hasBecier()) {
            _becierPipeline->bind(frameInfo.commandBuffer);
            frameInfo.commandBuffer.pushConstants(_pipelineLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(TextRendererPushConstants), &push);
            text.bindBecier(frameInfo.commandBuffer);
            text.drawBecier(frameInfo.commandBuffer);
        }

        _boxPipeline->bind(frameInfo.commandBuffer);
        frameInfo.commandBuffer.pushConstants(_pipelineLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(TextRendererPushConstants), &push);
        text.bindHard(frameInfo.commandBuffer);
        text.drawBox(frameInfo.commandBuffer);

    }
}