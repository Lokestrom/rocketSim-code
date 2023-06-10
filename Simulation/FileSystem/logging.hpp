#pragma once

#include "fileSystem.hpp"

namespace fileSystem {

	void loggCurrentState();
	void loggingStartup();
	void loggingEnd();

	void createGeneralRunInfo();

	void loggRocket(const Rocket& rocket);
	void loggRocketStage(const RocketStage& rocketStage, const String& globalID);
	void loggEngine(const Engine& engine, const String& globalID);
	void loggFuelTank(const FuelTank& fuelTank, const String& globalID);
	void loggPlanet(const PhysicsPlanet& planet);
	void loggPlanet(const FixedOrbitPlanet& planet);
}