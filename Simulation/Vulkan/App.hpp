#pragma once

#include "descriptors.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "systems/PointLightSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/RenderSystem2D.hpp"
#include "systems/TextSystem.hpp"
#include "windowFunctions/windowFunctions.hpp"
#include "PeripheralInputDevice.hpp"
#include "UI.hpp"

#include "../ObjectRenderingCashing.hpp"

// std
#include <chrono>
#include <memory>

class GameObject3D;

struct WindowInfo {
	unsigned int ID;
	windows::Type type;

	std::unique_ptr<Window> window;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Device> device;
	std::unique_ptr<DescriptorPool> globalPool;
	GameObject3D::Map gameObjects3d;
	UIElement::Map UIElements;
	StaticText::Map staticTexts;
	VaryingText<long double>::Map varyinglds;

	Vector<std::shared_ptr<TextInputField>> textInputFields;
	Vector<std::shared_ptr<Button>> buttons;
	
	std::optional<Background> background;

	std::vector<std::unique_ptr<Buffer>> uboBuffer;
	std::vector<vk::DescriptorSet> globalDescriptorSet;
	std::unique_ptr<RenderSystem3D> renderSystem3D;
	std::unique_ptr<RenderSystem2D> renderSystem2D;
	std::unique_ptr<PointLightSystem> pointLightSystem;
	std::unique_ptr<TextRenderer> textRenderSystem;

	std::unique_ptr<Camera> camera;

	std::chrono::steady_clock::time_point currentTime;

	void* typeSpecificInfo;

	static WindowInfo createWindowInfo(std::string name, windows::Type type, void* typeSpecificInfo = nullptr) {
		static unsigned int currentId = 0;
		return WindowInfo{ currentId++, name, type, typeSpecificInfo };
	}

	void prepereForTypeSwap();
	
	WindowInfo(const WindowInfo&) = delete;

	WindowInfo& operator=(WindowInfo&&) noexcept = default;
	WindowInfo(WindowInfo&&) noexcept;

	~WindowInfo();

	static WindowInfo& getWindowInfo(GLFWwindow* glfwWindow);

private:
	WindowInfo(unsigned int id, std::string name, windows::Type type, void* typeSpecificInfo);
};

class Vulkan
{
public:
	static constexpr int WIDTH = 1100;
	static constexpr int HEIGHT = 900;

	Vulkan();
	~Vulkan();

	Vulkan(const Vulkan&) = delete;
	Vulkan& operator=(const Vulkan&) = delete;

	static std::unordered_map<unsigned int, std::unique_ptr<WindowInfo>>& getWindows() { return _windows; };

	static void startup();
	static bool update();

	static void addWindow(WindowInfo window, void (*loadFunction)(WindowInfo&));

	static void resetCallback(GLFWwindow* window);

private:

	static void keyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseInput(GLFWwindow* window, int button, int action, int mods);

private:
	inline static std::unordered_map<unsigned int, std::unique_ptr<WindowInfo>> _windows;

	inline static Keyboard _keyboard;
	inline static Mouse _mouse;

	inline static SimulationTimeCash _currentSimulationState;

	inline static bool _pause;
};

