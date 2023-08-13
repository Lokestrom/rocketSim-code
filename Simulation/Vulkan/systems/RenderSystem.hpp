#pragma once

#include "../Camera.hpp"
#include "../Device.hpp"
#include "../FrameInfo.hpp"
#include "../GameObject.hpp"
#include "../Pipeline.hpp"

// std
#include <memory>
#include <vector>

class RenderSystem3D
{
public:
	RenderSystem3D(
		Device& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout);
	~RenderSystem3D();

	RenderSystem3D(const RenderSystem3D&) = delete;
	RenderSystem3D& operator=(const RenderSystem3D&) = delete;

	void renderGameObjects(FrameInfo& frameInfo);

private:
	void createPipelineLayout(vk::DescriptorSetLayout globalSetLayout);
	void createPipeline(vk::RenderPass renderPass);

	Device& _device;

	std::unique_ptr<Pipeline> _pipeline;
	vk::PipelineLayout _pipelineLayout;
};

