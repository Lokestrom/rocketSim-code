#pragma once

#include "Camera.hpp"
#include "GameObject.hpp"
#include "Text.hpp"

#include "vulkan/vulkan.hpp"

class GameObject3D;

struct PointLight {
	glm::vec4 position{};  // ignore w
	glm::vec4 color{};     // w is intensity
};

struct GlobalUbo {
	glm::mat4 projection{ 1.f };
	glm::mat4 view{ 1.f };
	glm::mat4 inverseView{ 1.f };
	glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity
	PointLight pointLights[10];
	glm::vec4 resolution;
	int numLights;
};

struct FrameInfo {
	int frameIndex;
	float frameTime;
	vk::CommandBuffer commandBuffer;
	Camera& camera;
	vk::DescriptorSet globalDescriptorSet;
	GameObject2D::Map& gameObjects2D;
	GameObject3D::Map& gameObjects3D;
	StaticText::Map& staticText;
	StaticText::MapRef varyingText;
};