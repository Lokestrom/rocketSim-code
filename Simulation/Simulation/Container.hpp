#pragma once
#include <thread>
#include <memory>

#include "ECS/Simulation.hpp"
#include "RenderingCache.hpp"
#include "CommandQueue.hpp"

#include <iostream>

class SimulationContainer {
public:
	SimulationContainer() = delete;

	SimulationContainer(const SimulationBuilder& builder)
		: simulation(std::make_unique<Simulation>(builder)), simThread(&SimulationContainer::simulationStart, this),
		running(true), commandQueue(*this)
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

	Simulation& getSimulation() noexcept {
		return *simulation;
	}

private:
	void simulationStart() {
		simulation->threadSetupOpenGLContext();
		loop();
	}

	void loop() {
		float nextLogTime = 0;
		while (running) {
			commandQueue.popCommands();
			if (renderingCache.isFull()) {
				// TODO: implement using std::condition_variable
				std::this_thread::yield();
				continue;
			}
			if (simulation->getTime() >= nextLogTime) {
				writeToCach();
				nextLogTime += 1.f/60.f;
			}
			simulation->update();
		}
	}

	void writeToCach() {
		RenderingCache::FrameData frameData;
		frameData.time = simulation->getTime();

		for (auto [id, pos, ori, scale, vel, angMom, model] : DataView<Position, Orientation, Scale, Velosity, AngularMomentum, Model3D>()) {
			frameData.components.emplace_back(RenderingCache::FrameData::ComponentData{
				.id = id,
				.position = pos,
				.orientation = ori,
				.scale = scale,
				.velocity = vel,
				.angularMomentum = angMom,
				//breaks when adding more models and a relocation happens
				.model = &model
				});
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