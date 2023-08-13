#pragma once
#include "../Camera.hpp"
#include "../Device.hpp"
#include "../FrameInfo.hpp"
#include "../GameObject.hpp"
#include "../Pipeline.hpp"

// std
#include <memory>
#include <vector>

class RenderSystem2D
{
	public:
		RenderSystem2D(Device& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout);
		~RenderSystem2D();

		RenderSystem2D(const RenderSystem2D&) = delete;
		RenderSystem2D& operator=(const RenderSystem2D&) = delete;

		void renderGameObjects(FrameInfo& frameInfo);

	private:
		void createPipelineLayout(vk::DescriptorSetLayout globalSetLayout);
		void createPipeline(vk::RenderPass renderPass);

	private:
		Device& _device;

		std::unique_ptr<Pipeline> _pipeline;
		vk::PipelineLayout _pipelineLayout;
};

