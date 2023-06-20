#pragma once

#include "fileSystem.hpp"

namespace fileSystem {

	void loggCurrentState();
	void loggingStartup();
	void loggingEnd();

	void createGeneralRunInfo();

	void createLoggingFilesForNewRocket(const Rocket& rocket);

	void createNewRocketLoggingFile(const String& ID);
	void createNewRocketStageLoggingFile(const String& ID);
	void createNewEngineLoggingFile(const String& ID);
	void createNewFuelTankLoggingFile(const String& ID);
	void createNewPlanetLoggingFile(const String& ID);

	void loggRocket(const Rocket& rocket);
	void loggRocketStage(const RocketStage& rocketStage, const String& globalID);
	void loggEngine(const Engine& engine, const String& globalID);
	void loggFuelTank(const FuelTank& fuelTank, const String& globalID);
	void loggPlanet(const PhysicsPlanet& planet);
	void loggPlanet(const FixedOrbitPlanet& planet);
}