#pragma once

#include "Device.hpp"

#include <string>
#include <vector>

struct PipelineConfigInfo {
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

    std::vector<vk::VertexInputBindingDescription> bindingDescriptions{};
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{};
    vk::PipelineViewportStateCreateInfo viewportInfo;
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
    vk::PipelineMultisampleStateCreateInfo multisampleInfo;
    std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachment;
    vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
    vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<vk::DynamicState> dynamicStateEnables;
    vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
    vk::PipelineLayout pipelineLayout = nullptr;
    vk::RenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class Pipeline
{
public:
    Pipeline(
        Device& device,
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo);
    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void bind(vk::CommandBuffer commandBuffer);

    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
    static void defaultPipelineConfigInfo3D(PipelineConfigInfo& configInfo);
    static void defaultPipelineConfigInfoText(PipelineConfigInfo& configInfo);
    static void defaultPipelineConfigInfo2D(PipelineConfigInfo& configInfo);
    static void enableAlphaBlending(PipelineConfigInfo& configInfo);

private:
    static std::vector<char> readFile(const std::string& filepath);

    void createGraphicsPipeline(
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo);

    vk::ShaderModule createShaderModule(const std::vector<char>& code);

    Device& _device;
    vk::Pipeline _graphicsPipeline;
    vk::ShaderModule _vertShaderModule;
    vk::ShaderModule _fragShaderModule;
};