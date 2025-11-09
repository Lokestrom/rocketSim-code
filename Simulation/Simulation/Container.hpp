#pragma once
#include <thread>
#include <memory>

#include "Simulation.hpp"
#include "RenderingCache.hpp"
#include "CommandQueue.hpp"

#include "Simulation/behaviors/model3D.hpp"

class SimulationContainer {
public:
	SimulationContainer() = delete;

	SimulationContainer(const SimulationBuilder& builder)
		: simulation(std::make_unique<Simulation>(builder)), simThread(&SimulationContainer::simulationStart, this),
		running(true), commandQueue(*simulation)
	{
	}

	~SimulationContainer() {
		if (running == true)
			end();
	}

	void end() {
		running = false;
		simThread.join();
		simulation.release();
	}

	RenderingCache& getRenderingCache() noexcept {
		return renderingCache;
	}

	CommandQueue& getCommandQueue() noexcept {
		return commandQueue;
	}

	//TODO: remove this
	Simulation& getSimulation() noexcept {
		return *simulation;
	}

private:
	void simulationStart() {
		simulation->threadSetupOpenGLContext();
		loop();
	}

	void loop() {
		float nextLoggTime = 0;
		while (running) {
			commandQueue.popCommands();
			if (renderingCache.isFull()) {
				// TODO: implement using std::condition_variable
				std::this_thread::yield();
				continue;
			}
			if (simulation->getTime() >= nextLoggTime) {
				writeToCach();
				nextLoggTime += 1/60;
			}
			simulation->update();
		}
	}

	void writeToCach() {
		RenderingCache::FrameData frameData;
		frameData.time = simulation->getTime();
		auto& model3DBehaviorManager = simulation->getBehaviorManagerSafe<Model3D>();

		for (const auto& comp : simulation->getComponents()) {
			if (!model3DBehaviorManager.getBehaviors().contains(comp->getID().getUUID()))
				continue;
			GPU::Model3D& model = model3DBehaviorManager.getModel3D(comp->getID().getUUID());
			RenderingCache::FrameData::ComponentData componentData{
				.id = comp->getID().getUUID(),
				.transform = comp->getTransformMat4(),
				.model = &model
			};
			frameData.components.push_back(componentData);
		}

		renderingCache.addFrameData(frameData);
	}

private:
	// simulation must be first so it is created first
	std::unique_ptr<Simulation> simulation;
	CommandQueue commandQueue;
	RenderingCache renderingCache;
	std::thread simThread;
	std::atomic<bool> running = false;
};