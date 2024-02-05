#pragma once

#include "fileSystem.hpp"

namespace fileSystem {

	void loggCurrentState();
	void loggingStartup();
	void loggingEnd();

	void createGeneralRunInfo();
	void loggError(Error error);

	void createLoggingFilesForNewRocket(const std::shared_ptr<Rocket> rocket);


	void createNewRocketLoggingFile(const IDview& ID);
	void createNewRocketStageLoggingFile(const IDview& ID);
	void createNewEngineLoggingFile(const IDview& ID);
	void createNewFuelTankLoggingFile(const IDview& ID);
	void createNewPlanetLoggingFile(const IDview& ID);

	void loggRocket(const Rocket& rocket);
	void loggRocketStage(const RocketStage& rocketStage);
	void loggEngine(const Engine& engine);
	void loggFuelTank(const FuelTank& fuelTank);
	void loggPlanet(const PhysicsPlanet& planet);
	void loggPlanet(const FixedOrbitPlanet& planet);
}