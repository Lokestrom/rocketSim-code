#include "Simulation.hpp"

#include "GLFW/glfw3.h"

Simulation::Simulation(const SimulationBuilder& builder)
	: _time(0), _dt(1E-3), _configPath(builder.configPath)
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	_openGLContext = glfwCreateWindow(1, 1, "test", nullptr, builder.mainOpenGLContext);
	assert(_openGLContext && "Simulation::Simulation: Failed to create OpenGL context for simulation");
}

Simulation::~Simulation() {
	// order of destruction matters here
	_components.clear();
	_behaviorManagers.clear();

	glfwDestroyWindow(_openGLContext);
}

void Simulation::threadSetupOpenGLContext() {
	assert(_openGLContext && "OpenGL context not created");
	glfwMakeContextCurrent(_openGLContext);
}

const std::filesystem::path& Simulation::getConfigPath() const noexcept
{
	return _configPath;
}

std::vector<std::unique_ptr<Component>>& Simulation::getComponents() noexcept
{
	return _components;
}

double Simulation::getTime() const noexcept
{
	return _time;
}

double Simulation::getDT() const noexcept {
	return _dt;
}


#ifdef OpenGLRendering
GLFWwindow* Simulation::getOpenGLContext() const noexcept {
	assert(_openGLContext && "OpenGL context not created");
	return _openGLContext;
}
#endif

void Simulation::update() {
	for (auto& [_, manager] : _behaviorManagers) {
		manager->update();
	}
	_time += _dt;
}

Component& Simulation::addComponent(std::unique_ptr<Component>&& component) {
	_components.push_back(std::move(component));
	return *_components.back();
}

void Simulation::removeComponent(const ID::UUID& id)
{
	static_assert(sizeof(ID::UUID) == sizeof(unsigned long long), "ID::GlobaleID size mismatch");
	assert([&]() {
		for (const auto& comp : _components) {
			if (comp->getID().getUUID() == id)
				return true;
		}
		return false;
	}() && "Simulation::removeComponent: No component with the given ID exists in the simulation");

	std::erase_if(_components,
		[&id](const std::unique_ptr<Component>& comp) {
			return comp->getID().getUUID() == id;
		}
	);
}