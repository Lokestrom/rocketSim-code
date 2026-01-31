#include "simBenchmark.hpp"
#include "ECS/Simulation.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Entities/PhysicsBody.hpp"

#include "helpers/physics.hpp"

#include <chrono>
#include <iostream>

void simulationBenchmarkRun() {

	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	std::cout << "Building the simulation...\n";
	Simulation sim = simBuilder();
	std::cout << "Simulation built.\n\n";
	std::cout << sim.getEntityContainer().size() << " entities in the simulation.\n\n";

	std::cout << "Warming up the simulation...\n";

	auto warmupStart = std::chrono::high_resolution_clock::now();
	{
		const size_t warmupSteps = static_cast<size_t>(1.0 / BenchmarkDT);
		for (size_t i = 0; i < warmupSteps; i++) {
			sim.update();
		}
	}
	
	auto warmupEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> warmupElapsed = warmupEnd - warmupStart;
	std::cout << "Warmup completed in " << warmupElapsed.count() << " seconds\n\n";

	if (performanceProfiling)
		profileSimulationBenchmarkRun( std::move(sim) );
	else
		timeSimulationBenchmarkRun( std::move(sim) );
}

void timeSimulationBenchmarkRun(Simulation&& sim)
{
	std::cout << "Starting benchmark for " << benchmarkTimeSeconds << " seconds of simulation time\n\n";

	const size_t totalSteps = static_cast<size_t>(benchmarkTimeSeconds / BenchmarkDT);

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < totalSteps; i++) {
		sim.update();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	double averageStepTime = elapsed.count() / static_cast<double>(totalSteps);
	std::cout << "Benchmark completed.\n";
	std::cout << "Total time: " << elapsed.count() << " seconds\n";
	std::cout << "Average step time: " << averageStepTime << " seconds\n";
	std::cout << "Steps per second: " << 1.0 / averageStepTime << " steps/second\n";
}

void profileSimulationBenchmarkRun(Simulation&& sim)
{
	std::cout << "Start profiler";

	for(;;)
		sim.update();

}

Simulation simBuilder() {
	SimulationBuilder builder;
	builder.configPath = "";
	builder.mainOpenGLContext = nullptr;

	Simulation sim(builder);
	sim.threadSetupOpenGLContext();
	gladLoadGL();

	PhysicsBodyBuilder largeBodyBuilder;
	largeBodyBuilder.name = "BenchmarkBody";
	largeBodyBuilder.localID = 0;
	largeBodyBuilder.modelPath = DEFAULT_CONFIG "model/sphere.obj";
	largeBodyBuilder.mass = 1.0e15;
	largeBodyBuilder.position = Vector3(0.0l, 0.0l, 0.0l);
	largeBodyBuilder.velocity = Vector3(0.0l, 0.0l, 0.0l);
	largeBodyBuilder.orientation = Quaternion(1.0l, 0.0l, 0.0l, 0.0l);
	largeBodyBuilder.angularVelocity = Vector3(1.0l, 0.0l, 1.0l);
	largeBodyBuilder.scale = Vector3(1.0l, 1.0l, 1.0l);
	PhysicsBody body(sim, largeBodyBuilder);

	PhysicsBodyBuilder smallBodyBuilder;
	smallBodyBuilder.name = "BenchmarkBodySmall";
	smallBodyBuilder.modelPath = DEFAULT_CONFIG "model/sphere.obj";
	smallBodyBuilder.mass = 1.0;
	smallBodyBuilder.orientation = Quaternion(1.0l, 0.0l, 0.0l, 0.0l);
	smallBodyBuilder.angularVelocity = Vector3(0.1l, 0.1l, 1.0l);
	smallBodyBuilder.scale = Vector3(0.1l, 0.1l, 0.1l);

	for( size_t i = 0; i < 50; i++ ) {
		smallBodyBuilder.position = Vector3(glm::sin(static_cast<long double>(i)) * 100.0l, glm::cos(static_cast<long double>(i)) * 100.0l, 0.0l);
		smallBodyBuilder.velocity = Vector3(0,0,glm::sqrt((G * largeBodyBuilder.mass)/100));
		smallBodyBuilder.localID = 1 + static_cast<ID::ID_T>(i);
		PhysicsBody smallBody(sim, smallBodyBuilder);
	}

	return sim;
}