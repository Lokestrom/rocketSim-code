#pragma once

#include <vector>
#include <memory>
#include <filesystem>
#include <array>
#include <type_traits>
#include <typeindex>

#include "Entity.hpp"
#include "DataRegistry.hpp"
#include "Integrator.hpp"
#include "types.hpp"

struct GLFWwindow;

struct SimulationBuilder {
	std::filesystem::path configPath;

#ifdef OpenGLRendering
	GLFWwindow* mainOpenGLContext = nullptr;
#endif
};

//note: this class may become a god class so important to not let that happen
class Simulation
{
public:
	Simulation() = delete;
	Simulation(const SimulationBuilder& builder);
	~Simulation();

	Simulation(const Simulation&) = delete;
	Simulation& operator=(const Simulation&) = delete;
	Simulation(Simulation&&) = default;
	Simulation& operator=(Simulation&&) = default;

	void threadSetupOpenGLContext();

	const std::filesystem::path& getConfigPath() const noexcept;
	double getTime() const noexcept;
	double getDT() const noexcept;

	void update();

	EntityContainer& getEntityContainer() noexcept;
	IntegratorContainer<RK4Integrator>& getIntegratorContainer() noexcept;
	
#ifdef OpenGLRendering
	GLFWwindow* getOpenGLContext() const noexcept;
#endif

private:
#ifdef OpenGLRendering
	GLFWwindow* _openGLContext = nullptr;
#endif
	EntityContainer _entities;
	IntegratorContainer<RK4Integrator> _integrators;
	double _time;
	double _dt;

	std::filesystem::path _configPath;
};