#include "Simulation.hpp"

#include "GLFW/glfw3.h"

#include "helpers/physics.hpp"
#include "Integrator.hpp"
#include "types.hpp"
#include <algorithm>
#include <thread>

Simulation::Simulation(const SimulationBuilder& builder)
	: _time(0), _dt(1E-3), _configPath(builder.configPath)
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	_openGLContext = glfwCreateWindow(1, 1, "test", nullptr, builder.mainOpenGLContext);
	assert(_openGLContext && "Simulation::Simulation: Failed to create OpenGL context for simulation");
}

Simulation::~Simulation() {
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
	//can be parallelized later
	//update phase then late update phase to allow for interdependent calculations
	for (auto [id, integrator] : _integrators) {
		integrator.update(*this, id);
	}
	for (auto [id, integrator] : _integrators) {
		integrator.lateUpdate(*this, id);
	}
	_time += _dt;
}

EntityContainer& Simulation::getEntityContainer() noexcept
{
	return _entities;
}

IntegratorContainer<RK4Integrator>& Simulation::getIntegratorContainer() noexcept
{
	return _integrators;
}
