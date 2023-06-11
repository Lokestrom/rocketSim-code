#pragma once

#include "fileSystem.hpp"

namespace fileSystem {

	void loadInObjects();

	Shape loadMesh(String meshFile);

	void validateEngineVariables(std::unordered_map<String, String> map);
	Engine loadEngine(String engineFile, bool reactionThruster);

	void validateFuelTankVariables(std::unordered_map<String, String> map);
	FuelTank loadFuelTank(String FuelTankFile);

	void validateRocketStageVariables(std::unordered_map<String, std::unordered_map<String, String>> map);
	RocketStage loadRocketStage(String rocketStageFile);

	void validatePlanetFileVariables(std::unordered_map<String, std::unordered_map<String, String>> map);
	bool loadPlanet(String planetFile, PhysicsPlanet& physicsPlanet, FixedOrbitPlanet& fixedOrbitPlanet);

	void validateRocketVariables(std::unordered_map<String, std::unordered_map<String, String>> map, bool& onPlanetStart);
	Rocket loadRocket(String rocketFile);

	void validateSettingVariables(std::unordered_map<String, String> map);
	void validateAllLoadedObjects(std::unordered_map<String, String> settings);
	void loadSettings(String settingsFile);

	std::unordered_map<String, String> loadVariablesAndValuesInToMap(std::ifstream& file);
	std::unordered_map<String, std::unordered_map<String, String>> loadBatches(std::ifstream& file);

	namespace LoadManagerMaps {
		inline std::unordered_map<String, Shape> mesh;

		inline std::unordered_map<String, Engine> engine;
		inline std::unordered_map<String, ReactionThruster> reactionThruster;
		inline std::unordered_map<String, FuelTank> fuelTank;
		inline std::unordered_map<String, RocketStage> rocketStage;

		inline std::unordered_map<String, PhysicsPlanet> pysicsPlanet;
		inline std::unordered_map<String, FixedOrbitPlanet> fixedOrbitPlanet;
	}
} // fileSystem