#pragma once

#include "../Camera.hpp"
#include "../Device.hpp"
#include "../FrameInfo.hpp"
#include "../GameObject.hpp"
#include "../Pipeline.hpp"

// std
#include <memory>
#include <vector>

class PointLightSystem
{
public:
	PointLightSystem(Device& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout);
	~PointLightSystem();

	PointLightSystem(const PointLightSystem&) = delete;
	PointLightSystem& operator=(const PointLightSystem&) = delete;

	void update(FrameInfo& frameInfo, GlobalUbo& ubo);
	void render(FrameInfo& frameInfo);

private:
	void createPipelineLayout(vk::DescriptorSetLayout globalSetLayout);
	void createPipeline(vk::RenderPass renderPass);

private:
	Device& _device;

	std::unique_ptr<Pipeline> _pipeline;
	vk::PipelineLayout _pipelineLayout;
};

