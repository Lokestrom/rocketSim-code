#include "Pipeline.hpp"

#include "Model.hpp"
#include "Text.hpp"

// std
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

Pipeline::Pipeline(
    Device& device,
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipelineConfigInfo& configInfo)
    : _device{ device } {
    createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

Pipeline::~Pipeline() {
    _device.device().destroyShaderModule(_vertShaderModule);
    _device.device().destroyShaderModule(_fragShaderModule);
    _device.device().destroyPipeline(_graphicsPipeline);
}

std::vector<char> Pipeline::readFile(const std::string& filepath) {
    std::string enginePath = filepath;
    std::ifstream file{ enginePath, std::ios::ate | std::ios::binary };

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + enginePath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

void Pipeline::createGraphicsPipeline(
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipelineConfigInfo& configInfo) {
    assert(
        configInfo.pipelineLayout != NULL &&
        "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
    assert(
        configInfo.renderPass != NULL &&
        "Cannot create graphics pipeline: no renderPass provided in configInfo");

    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    _vertShaderModule = createShaderModule(vertCode);
    _fragShaderModule = createShaderModule(fragCode);

    vk::PipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].stage = vk::ShaderStageFlagBits::eVertex;
    shaderStages[0].module = _vertShaderModule;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = vk::PipelineShaderStageCreateFlags();
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;
    shaderStages[1].stage = vk::ShaderStageFlagBits::eFragment;
    shaderStages[1].module = _fragShaderModule;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = vk::PipelineShaderStageCreateFlags();
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    auto& bindingDescriptions = configInfo.bindingDescriptions;
    auto& attributeDescriptions = configInfo.attributeDescriptions;

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

    vk::GraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &configInfo.viewportInfo;
    pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
    pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

    pipelineInfo.layout = configInfo.pipelineLayout;
    pipelineInfo.renderPass = configInfo.renderPass;
    pipelineInfo.subpass = configInfo.subpass;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = nullptr;

    if (_device.device().createGraphicsPipelines(nullptr, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create graphics pipeline");
    }
}

vk::ShaderModule Pipeline::createShaderModule(const std::vector<char>& code) {
    vk::ShaderModuleCreateInfo createInfo{};
    createInfo.flags = vk::ShaderModuleCreateFlags();
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    return _device.device().createShaderModule(createInfo);
}

void Pipeline::bind(vk::CommandBuffer commandBuffer) {
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _graphicsPipeline);
}

void Pipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
{
    configInfo.inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo.viewportInfo.viewportCount = 1;
    configInfo.viewportInfo.pViewports = nullptr;
    configInfo.viewportInfo.scissorCount = 1;
    configInfo.viewportInfo.pScissors = nullptr;

    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = vk::PolygonMode::eFill;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = vk::CullModeFlags();
    configInfo.rasterizationInfo.frontFace = vk::FrontFace::eClockwise;
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
    configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
    configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

    configInfo.colorBlendAttachment.push_back(vk::PipelineColorBlendAttachmentState{
        VK_FALSE,
        vk::BlendFactor::eOne,   // Optional
        vk::BlendFactor::eZero,  // Optional
        vk::BlendOp::eAdd,              // Optional
        vk::BlendFactor::eOne,   // Optional
        vk::BlendFactor::eZero,  // Optional
        vk::BlendOp::eAdd,             // Optional
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
    });

    configInfo.colorBlendAttachment.push_back(vk::PipelineColorBlendAttachmentState{
        VK_FALSE,
        vk::BlendFactor::eOne,   // Optional
        vk::BlendFactor::eZero,  // Optional
        vk::BlendOp::eAdd,              // Optional
        vk::BlendFactor::eOne,   // Optional
        vk::BlendFactor::eZero,  // Optional
        vk::BlendOp::eAdd,            // Optional
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
    });

    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = vk::LogicOp::eCopy;  // Optional
    configInfo.colorBlendInfo.attachmentCount = static_cast<uint32_t>(configInfo.colorBlendAttachment.size());
    configInfo.colorBlendInfo.pAttachments = configInfo.colorBlendAttachment.data();
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional  

    configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthCompareOp = vk::CompareOp::eLess;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.front = vk::StencilOpState();  // Optional
    configInfo.depthStencilInfo.back = vk::StencilOpState();   // Optional

    configInfo.dynamicStateEnables = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
    configInfo.dynamicStateInfo.dynamicStateCount =
        static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
    configInfo.dynamicStateInfo.flags = vk::PipelineDynamicStateCreateFlags();
}

void Pipeline::defaultPipelineConfigInfo3D(PipelineConfigInfo& configInfo) {
    defaultPipelineConfigInfo(configInfo);
    configInfo.bindingDescriptions = Model3D::Vertex::getBindingDescriptions();
    configInfo.attributeDescriptions = Model3D::Vertex::getAttributeDescriptions();
}

void Pipeline::defaultPipelineConfigInfoText(PipelineConfigInfo& configInfo)
{
    defaultPipelineConfigInfo(configInfo);
    configInfo.bindingDescriptions = StaticText::Vertex::getBindingDescriptions();
    configInfo.attributeDescriptions = StaticText::Vertex::getAttributeDescriptions();
    configInfo.depthStencilInfo.front = vk::StencilOpState();
    configInfo.depthStencilInfo.front.setReference(0);
    configInfo.depthStencilInfo.front.setCompareOp(vk::CompareOp::eAlways);
    configInfo.depthStencilInfo.front.setFailOp(vk::StencilOp::eInvert);
    configInfo.depthStencilInfo.front.setPassOp(vk::StencilOp::eInvert);

    configInfo.depthStencilInfo.back = vk::StencilOpState();
    configInfo.depthStencilInfo.back.setReference(0);
    configInfo.depthStencilInfo.back.setCompareOp(vk::CompareOp::eAlways);
    configInfo.depthStencilInfo.back.setFailOp(vk::StencilOp::eInvert);
    configInfo.depthStencilInfo.back.setPassOp(vk::StencilOp::eInvert);

    configInfo.depthStencilInfo.depthCompareOp = vk::CompareOp::eAlways;
    configInfo.depthStencilInfo.stencilTestEnable = VK_TRUE;
    configInfo.dynamicStateEnables = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
    configInfo.dynamicStateInfo.dynamicStateCount =
        static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
}

void Pipeline::defaultPipelineConfigInfo2D(PipelineConfigInfo& configInfo) {
    defaultPipelineConfigInfo(configInfo);
    configInfo.bindingDescriptions = Model2D::Vertex::getBindingDescriptions();
    configInfo.attributeDescriptions = Model2D::Vertex::getAttributeDescriptions();
    enableAlphaBlending(configInfo);
}

void Pipeline::enableAlphaBlending(PipelineConfigInfo& configInfo) {
    configInfo.colorBlendAttachment[0].blendEnable = VK_TRUE;
    configInfo.colorBlendAttachment[0].colorWriteMask =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    configInfo.colorBlendAttachment[0].srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    configInfo.colorBlendAttachment[0].dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
    configInfo.colorBlendAttachment[0].colorBlendOp = vk::BlendOp::eAdd;
    configInfo.colorBlendAttachment[0].srcAlphaBlendFactor = vk::BlendFactor::eOne;
    configInfo.colorBlendAttachment[0].dstAlphaBlendFactor = vk::BlendFactor::eZero;
    configInfo.colorBlendAttachment[0].alphaBlendOp = vk::BlendOp::eAdd;
}