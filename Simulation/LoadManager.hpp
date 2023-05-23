#pragma once
#include <unordered_map>
#include "String.hpp"
#include "Mesh.hpp"
#include "Engine.hpp"
#include "FuelTank.hpp"

namespace LoadManagerObjects{
	std::unordered_map<String, Shape> meshMap;
	std::unordered_map<String, Engine> engineMap;
	std::unordered_map<String, ReactionThruster> reactionThrusterMap;
	std::unordered_map<String, FuelTank> fuelTankMap;
	std::unordered_map<String, RocketStage> rocketStageMap;

	std::unordered_map<String, FixedOrbitPlanet> fixedOrbitPlanetMap;
	std::unordered_map<String, PhysicsPlanet> pysicsPlanetMap;

}