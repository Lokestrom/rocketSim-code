#pragma once
#include "../Camera.hpp"
#include "../Device.hpp"
#include "../FrameInfo.hpp"
#include "../GameObject.hpp"
#include "../Pipeline.hpp"

// std
#include <memory>
#include <vector>

class TextRenderer
{
public:
	TextRenderer(Device& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout);
	~TextRenderer();

	TextRenderer(const TextRenderer&) = delete;
	TextRenderer& operator=(const TextRenderer&) = delete;

	void renderText(FrameInfo& frameInfo);

private:
	void createPipelineLayout(vk::DescriptorSetLayout globalSetLayout);
	void createPipeline(vk::RenderPass renderPass);

private:
	Device& _device;

	std::unique_ptr<Pipeline> _pipeline;
	vk::PipelineLayout _pipelineLayout;
};

