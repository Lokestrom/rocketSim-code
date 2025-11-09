#pragma once

#include <vector>
#include <memory>
#include <filesystem>
#include <array>
#include <type_traits>
#include <typeindex>

#include "Component.hpp"
#include "Behavior.hpp"

struct GLFWwindow;

struct SimulationBuilder {
	std::filesystem::path configPath;

#ifdef OpenGLRendering
	GLFWwindow* mainOpenGLContext = nullptr;
#endif
};

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

	template<isBehavior B>
	constexpr auto& getBehaviorManagerSafe();

	void threadSetupOpenGLContext();

	const std::filesystem::path& getConfigPath() const noexcept;
	std::vector<std::unique_ptr<Component>>& getComponents() noexcept;
	double getTime() const noexcept;
	double getDT() const noexcept;

	void update();

	template<class T>
		requires std::is_base_of_v<Component, T>
	T& addComponent(T&& component);
	Component& addComponent(std::unique_ptr<Component>&& component);

	void removeComponent(const ID::UUID& id);
	
#ifdef OpenGLRendering
	GLFWwindow* getOpenGLContext() const noexcept;
#endif

private:
#ifdef OpenGLRendering
	GLFWwindow* _openGLContext = nullptr;
#endif
	std::unordered_map<std::type_index, std::unique_ptr<IBehaviorManager>> _behaviorManagers;
	std::vector<std::unique_ptr<Component>> _components;
	double _time;
	double _dt;

	std::filesystem::path _configPath;
};

template<isBehavior B>
constexpr auto& Simulation::getBehaviorManagerSafe() {
	using Family = typename B::FamilyType;
	using ManagerType = typename Family::ManagerType;

	auto key = std::type_index(typeid(Family));
	auto it = _behaviorManagers.find(key);

	if (it == _behaviorManagers.end()) {
		auto mgr = std::make_unique<ManagerType>(*this);
		auto* ptr = mgr.get();
		_behaviorManagers.emplace(key, std::move(mgr));
		return *ptr;
	}

	return *static_cast<ManagerType*>(it->second.get());
}

template<class T>
	requires std::is_base_of_v<Component, T>
T& Simulation::addComponent(T&& component) {
	std::unique_ptr<T> ptr = std::make_unique<T>(std::move(component));
	T& ref = *ptr;
	_components.push_back(std::move(ptr));
	return ref;
}