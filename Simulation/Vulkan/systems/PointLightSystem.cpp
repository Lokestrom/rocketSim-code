#include "PointLightSystem.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <map>
#include <stdexcept>

class GameObject;

struct PointLightPushConstants {
    glm::vec4 position{};
    glm::vec4 color{};
    float radius;
};

glm::vec4 toVec4(Vector3 v, double r) {
    return glm::vec4{ v.x,v.y,v.z,r };
}

Vector3 toVector3(glm::vec3 v) {
    return Vector3{ v.x,v.y,v.z };
}
glm::vec3 toVec3(Vector3 v) {
    return glm::vec3{ v.x,v.y,v.z };
}

PointLightSystem::PointLightSystem(
    Device& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout)
    : _device{ device } {
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

PointLightSystem::~PointLightSystem() {
    _device.device().destroyPipelineLayout(_pipelineLayout);
}

void PointLightSystem::createPipelineLayout(vk::DescriptorSetLayout globalSetLayout) {
    vk::PushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PointLightPushConstants);
    

    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = vk::StructureType::ePipelineLayoutCreateInfo;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (_device.device().createPipelineLayout(&pipelineLayoutInfo, nullptr, &_pipelineLayout) != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
    
}

void PointLightSystem::createPipeline(vk::RenderPass renderPass) {
    assert(_pipelineLayout != NULL && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo3D(pipelineConfig);
    Pipeline::enableAlphaBlending(pipelineConfig);
    pipelineConfig.attributeDescriptions.clear();
    pipelineConfig.bindingDescriptions.clear();
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = _pipelineLayout;
    _pipeline = std::make_unique<Pipeline>(
        _device,
        "D:\\code\\codeProjects\\VulkanTest\\shaders\\point_light.vert.spv",
        "D:\\code\\codeProjects\\VulkanTest\\shaders\\point_light.frag.spv",
        pipelineConfig);
}

void PointLightSystem::render(FrameInfo& frameInfo) {
    // sort lights
    std::map<float, GameObject3D::id_t> sorted;
    for (auto& kv : frameInfo.gameObjects3D) {
        auto& obj = kv.second;
        if (obj.pointLight == nullptr) continue;

        // calculate distance
        auto offset = frameInfo.camera.getPosition() - toVec3(obj.translation());
        float disSquared = glm::dot(offset, offset);
        sorted[disSquared] = obj.getId();
    }

    _pipeline->bind(frameInfo.commandBuffer);
    frameInfo.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    // iterate through sorted lights in reverse order
    for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
        // use game obj id to find light object
        auto& obj = frameInfo.gameObjects3D.at(it->second);

        PointLightPushConstants push{};
        push.position = glm::vec4(toVec4(obj.translation(), 1.f));
        push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
        push.radius = obj.transform.scale.x;

        frameInfo.commandBuffer.pushConstants(_pipelineLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(PointLightPushConstants), &push);
        frameInfo.commandBuffer.draw(6, 1, 0, 0);
    }
}