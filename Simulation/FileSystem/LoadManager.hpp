#pragma once

#include "fileSystem.hpp"

namespace fileSystem {

	void loadInObjects();

	Shape loadMesh(String meshFile);

	void validateEngineVariables(std::unordered_map<String, String> map);
	Engine::Builder loadEngine(String engineFile);
	void validateFuelTankVariables(std::unordered_map<String, String> map);
	FuelTank::Builder loadFuelTank(String FuelTankFile);

	void validateRocketStageVariables(std::unordered_map<String, std::unordered_map<String, String>> map);
	RocketStage::Builder loadRocketStage(String rocketStageFile);

	void validatePlanetFileVariables(std::unordered_map<String, std::unordered_map<String, String>> map);
	bool loadPlanet(String planetFile, PhysicsPlanet::Builder& physicsPlanet, FixedOrbitPlanet::Builder& fixedOrbitPlanet);

	void validateRocketVariables(std::unordered_map<String, std::unordered_map<String, String>> map, bool& onPlanetStart);
	Rocket::Builder loadRocket(String rocketFile);

	void validateSettingVariables(std::unordered_map<String, String> map);
	void validateAllLoadedObjects(std::unordered_map<String, String> settings);
	void loadSettings(String settingsFile);

	void loadingFinishingTouches();

	std::unordered_map<String, String> loadVariablesAndValuesInToMap(std::ifstream& file);
	std::unordered_map<String, std::unordered_map<String, String>> loadBatches(std::ifstream& file);

	namespace LoadManagerMaps {
		inline std::unordered_map<String, Shape> mesh;
		inline std::unordered_map<String, Model3D::Builder> model;

		inline std::unordered_map<String, Engine::Builder> engine;
		inline std::unordered_map<String, FuelTank::Builder> fuelTank;
		inline std::unordered_map<String, RocketStage::Builder> rocketStage;
		inline std::unordered_map<String, Rocket::Builder> rocket;

		inline std::unordered_map<String, PhysicsPlanet::Builder> pysicsPlanet;
		inline std::unordered_map<String, FixedOrbitPlanet::Builder> fixedOrbitPlanet;
	}
} // fileSystem