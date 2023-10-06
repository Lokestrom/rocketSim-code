#include "LoadManager.hpp"
#include "Instructions.hpp"

#include <iostream>

namespace fs = std::filesystem;
namespace fileSystem {
	void loadInObjects() {
		objects::simulationFolder += "\\config\\";
		for (const auto& entry : fs::directory_iterator(toSTD(objects::simulationFolder + "mesh")))
			LoadManagerMaps::mesh[String(entry.path().filename().string()).split('.')[0]] = loadMesh(entry.path().string());
		Model3D::Builder model;
		for (const auto& entry : fs::directory_iterator(toSTD(objects::simulationFolder + "model"))) {
			model.loadModel(entry.path().string());
			LoadManagerMaps::model[String(entry.path().filename().string()).split('.')[0]] = std::move(model);
		}
		for (const auto& entry : fs::directory_iterator(toSTD(objects::simulationFolder + "rocket\\engine"))) {
			Engine::Builder engine = loadEngine(entry.path().string());
			LoadManagerMaps::engine[engine.name] = engine;
		}
		for (const auto& entry : fs::directory_iterator(toSTD(objects::simulationFolder + "rocket\\fuelTank")))
			LoadManagerMaps::fuelTank[String(entry.path().filename().string()).split('.')[0]] = loadFuelTank(entry.path().string());
		for (const auto& entry : fs::directory_iterator(toSTD(objects::simulationFolder + "planet"))) {
			PhysicsPlanet::Builder physicsPlanet;
			FixedOrbitPlanet::Builder fixedOrbitPlanet;
			bool isFixedOrbit = loadPlanet(entry.path().string(), physicsPlanet, fixedOrbitPlanet);

			if (isFixedOrbit)
				LoadManagerMaps::fixedOrbitPlanet[String(entry.path().filename().string()).split('.')[0]] = fixedOrbitPlanet;
			else
				LoadManagerMaps::pysicsPlanet[String(entry.path().filename().string()).split('.')[0]] = physicsPlanet;
		}
		for (const auto& entry : fs::directory_iterator(toSTD(objects::simulationFolder + "rocket\\rocketStage")))
			LoadManagerMaps::rocketStage[String(entry.path().filename().string()).split('.')[0]] = loadRocketStage(entry.path().string());
		for (const auto& entry : fs::directory_iterator(toSTD(objects::simulationFolder + "rocket")))
			if (!entry.is_directory()) {
				objectLists::rockets.pushBack(std::make_shared<Rocket>(loadRocket(entry.path().string())));
				objectLists::instructions.pushBack(std::make_shared<Instructions>(objectLists::rockets.at(objectLists::rockets.size() - 1)->getID().getName() + ".txt", objectLists::rockets.at(objectLists::rockets.size() - 1)));
			}
		for (const auto& [key, val] : LoadManagerMaps::fixedOrbitPlanet) {
			objectLists::fixedOrbitPlanets.pushBack(std::make_shared<FixedOrbitPlanet>(val));
		}
		for (const auto& [key, val] : LoadManagerMaps::pysicsPlanet) {
			objectLists::physicsPlanets.pushBack(std::make_shared<PhysicsPlanet>(val));
		}
		loadSettings(objects::simulationFolder + "settings.txt");

		loadingFinishingTouches();
	}

	Shape loadMesh(String meshFile) {
		std::ifstream file(meshFile.cstr());
		String line;
		Shape shape;
		for (auto i = 0; getline(file, line); i++) {
			String type = line.split('(')[0];
			type.lower();
			String args = line.split('(')[1];
			Vector<String> argv = returnArgs(args);
			bool solid = (argv.size() > 3) ? ((argv[3] == "true") ? true : false) : true;
			if (type == "sphere") {
				shape.meshes.pushBack(ShapeNode(MeshType::Sphere,
					Sphere(returnVector3(argv[0]), STold(argv[1]), solid),
					Box(),
					Cylinder(),
					NoseCone()
				));
			}
			else if (type == "box") {
				shape.meshes.pushBack(ShapeNode(MeshType::Box,
					Sphere(),
					Box(returnVector3(argv[0]), returnVector3(argv[1]), returnQuaternion(argv[2]), solid),
					Cylinder(),
					NoseCone()
				));
			}
			else if (type == "cylinder") {
				shape.meshes.pushBack(ShapeNode(MeshType::Cylinder,
					Sphere(),
					Box(),
					Cylinder(returnVector3(argv[0]), STold(argv[1]), STold(argv[2]), solid),
					NoseCone()
				));
			}
			else if (type == "nosecone") {
				shape.meshes.pushBack(ShapeNode(MeshType::NoseCone,
					Sphere(),
					Box(),
					Cylinder(),
					NoseCone(returnVector3(argv[0]), STold(argv[1]), STold(argv[2]), solid)
				));
			}
			else {
				file.close();
				throw error("The mesh type: " + type + ". Is not a valid mesh", exitCodes::badUserBehavior);

			}
		}
		file.close();
		return shape;
	}

	bool validationLayer(const std::unordered_map<String, String>& map, String variable, String& msg) {
		variable.remove(' ');
		if (!map.count(lower(variable))) {
			msg += "\t" + variable + ",\n";
			return false;
		}
		return true;
	}

	bool validationLayer(const std::unordered_map<String, std::unordered_map<String, String>>& map, String variable, String& msg) {
		variable.remove(' ');
		if (!map.count(lower(variable))) {
			msg += "\t" + variable + ",\n";
			return false;
		}
		return true;
	}

	template<typename T>
	void validateThatObjectshasBeenLoaded(const std::unordered_map<String, String>& map, const std::unordered_map<String, T>& fileMap, String& msg) {
		for (const auto& [key, val] : map) {
			if (!fileMap.count(val.split('(')[0]))
				msg += "\t" + val + ",\n";
		}
	}

	void validateThatValueIsNotNegative(std::unordered_map<String, String> map, const String& val) {
		if (STold(map[val]) < 0)
			throw error((val + " can't be negative."), exitCodes::badUserBehavior);
	}

	void validateEngineVariables(std::unordered_map<String, String> map) {
		String misingValues;
		validationLayer(map, "fuel per second", misingValues);
		validationLayer(map, "mesh", misingValues);
		validationLayer(map, "model", misingValues);
		validationLayer(map, "mass", misingValues);
		validationLayer(map, "exit velosity", misingValues);
		validationLayer(map, "center of mass", misingValues);
		validationLayer(map, "mount pos", misingValues);
		if (misingValues != "")
			throw error("There is mising values in the Engine file:\n" + misingValues + "Check for spelling errors.", exitCodes::badUserBehavior);
		if (!LoadManagerMaps::mesh.count(map["mesh"]))
			throw error("The mesh: " + map["mesh"] + " has not been loaded. Check for spelling errors", exitCodes::badUserBehavior);
		if (!LoadManagerMaps::model.count(map["model"]))
			throw error("The model: " + map["model"] + " has not been loaded. Check for spelling errors", exitCodes::badUserBehavior);

		try {
			validateThatValueIsNotNegative(map, "mass");
			validateThatValueIsNotNegative(map, "exitvelosity");
			validateThatValueIsNotNegative(map, "fuelpersecond");
		}
		catch (error& e) {
			throw e;
		}

		if (!map.count("maxgimblepersecond") && map.count("maxgimble"))
			throw error("there is a value for max gimble but not for max gimble per second. you may have forgoten the max gimble per second variable", exitCodes::badUserBehavior);
		if (map.count("maxgimblepersecond") && !map.count("maxgimble"))
			throw error("there is a value for max gimble per second but not for max gimble. you may have forgoten the max gimble variable", exitCodes::badUserBehavior);

	}

	Engine::Builder loadEngine(String engineFile) {
		std::ifstream file(engineFile.cstr());
		String line;
		std::unordered_map<String, String> map = loadVariablesAndValuesInToMap(file);
		file.close();
		try {
			validateEngineVariables(map);
		}
		catch (error& e) {
			e.what = "Error in file: " + engineFile + ":\n" + e.what;
			throw e;
		}
		Engine::Builder engine;
		engine.name = engineFile.split('\\')[engineFile.split('\\').size() - 1].split('.')[0];
		engine.mass = STold(map["mass"]);
		engine.exitVel = STold(map["exitvelosity"]);
		engine.centerOfMass = returnVector3(map["centerofmass"]);
		engine.mountPos = returnVector3(map["mountpos"]);
		Vector<String> fuelPerSecondArgs = map["fuelpersecond"].split(',');
		for (const auto& i : fuelPerSecondArgs) {
			Vector<String> fuel = i.split(':');
			engine.fuelPerSecond.addFuel(Fuelmap(fuel[0], STold(fuel[1])));
		}
		engine.shape = LoadManagerMaps::mesh[map["mesh"]];
		engine.model = LoadManagerMaps::model[map["model"]];
		if (!map.count("maxgimblepersecond") && !map.count("maxgimble")) {
			engine.maxGimble = 0;
			engine.maxGimblePerSecond = 0;
		}
		return engine;
	}

	void validateFuelTankVariables(std::unordered_map<String, String> map) {
		String misingValues;
		validationLayer(map, "fuel type", misingValues);
		validationLayer(map, "radius", misingValues);
		validationLayer(map, "height", misingValues);
		validationLayer(map, "fuel density", misingValues);
		validationLayer(map, "fuelmass", misingValues);
		validationLayer(map, "model", misingValues);
		if (misingValues != "")
			throw error("There is mising values in the file:\n" + misingValues + "Check for spelling errors.", exitCodes::badUserBehavior);
		if (!LoadManagerMaps::model.count(map["model"]))
			throw error("The model: " + map["model"] + "has not been loaded. Check for spelling errors", exitCodes::badUserBehavior);

		try {
			validateThatValueIsNotNegative(map, "fuelmass");
			validateThatValueIsNotNegative(map, "fueldensity");
			validateThatValueIsNotNegative(map, "radius");
			validateThatValueIsNotNegative(map, "height");
		}
		catch (error& e) {
			throw e;
		}

		if (PI * STold(map["radius"]) * STold(map["radius"]), STold(map["height"]) < STold(map["fuelmass"])/STold(map["fueldensity"]))
			throw error("There is more mass in the tank than there is volum. Decrease the fuel amont, increase the density or make the fuel tank bigger", exitCodes::badUserBehavior);
	}

	FuelTank::Builder loadFuelTank(String FuelTankFile) {
		std::ifstream file(FuelTankFile.cstr());
		String line;
		std::unordered_map<String, String> map = loadVariablesAndValuesInToMap(file);
		file.close();
		try {
			validateFuelTankVariables(map);
		}
		catch (error& e) {
			e.what = "Error in file: " + FuelTankFile + ":\n" + e.what;
			throw e;
		}

		FuelTank::Builder fuelTank;
		fuelTank.name = FuelTankFile.split('\\')[FuelTankFile.split('\\').size() - 1].split('.')[0];
		fuelTank.fuelType = map["fueltype"];
		fuelTank.fuelLoad = STold(map["fuelmass"]);
		fuelTank.fuelDensity = STold(map["fueldensity"]);
		fuelTank.radius = STold(map["radius"]);
		fuelTank.height = STold(map["height"]);
		fuelTank.model = LoadManagerMaps::model[map["model"]];
		
		return fuelTank;
	}	

	void validateRocketStageVariables(std::unordered_map<String, std::unordered_map<String, String>> map) {
		String misingBatches;
		validationLayer(map, "setup", misingBatches);
		validationLayer(map, "engines", misingBatches);
		validationLayer(map, "reaction thrusters", misingBatches);
		validationLayer(map, "fuel tanks", misingBatches);
		if (misingBatches != "")
			throw error("There is mising batches in the file:\n" + misingBatches + "Check for spelling errors.", exitCodes::badUserBehavior);

		String misingValues;
		validationLayer(map["setup"], "center of mass", misingValues);
		validationLayer(map["setup"], "mesh", misingValues);
		validationLayer(map["setup"], "dry mass", misingValues);
		validationLayer(map["setup"], "model", misingValues);
		if (misingValues != "")
			throw error("There is mising values in the file:\n" + misingValues + "Check for spelling errors.", exitCodes::badUserBehavior);
		if (!LoadManagerMaps::mesh.count(map["setup"]["mesh"]))
			throw error("The mesh: " + map["setup"]["mesh"] + "has not been loaded. Check for spelling errors", exitCodes::badUserBehavior);
		if (!LoadManagerMaps::model.count(map["setup"]["model"]))
			throw error("The model: " + map["setup"]["model"] + "has not been loaded. Check for spelling errors", exitCodes::badUserBehavior);
		try {
			validateThatValueIsNotNegative(map["setup"], "drymass");
		}
		catch (error& e) {
			throw e;
		}

		String notLoadedObjects;
		validateThatObjectshasBeenLoaded(map["engines"], LoadManagerMaps::engine, notLoadedObjects);
		validateThatObjectshasBeenLoaded(map["fueltanks"], LoadManagerMaps::fuelTank, notLoadedObjects);

		if(notLoadedObjects != "")
			throw error("Therese objects are mising: \n" + notLoadedObjects + "Check for spelling errors", exitCodes::badUserBehavior);
	}

	RocketStage::Builder loadRocketStage(String rocketStageFile) {
		std::ifstream file(rocketStageFile.cstr());
		auto data = loadBatches(file);
		validateRocketStageVariables(data);
		file.close();
		
		RocketStage::Builder rocketStage;
		rocketStage.name = rocketStageFile.split('\\')[rocketStageFile.split('\\').size() - 1].split('.')[0];
		rocketStage.dryMass = STold(data["setup"]["drymass"]);
		rocketStage.centerOfMass = returnVector3(data["setup"]["centerofmass"]);
		rocketStage.mesh = LoadManagerMaps::mesh[data["setup"]["mesh"]];
		rocketStage.model = LoadManagerMaps::model[data["setup"]["model"]];

		for (auto i = 0; i < data["engines"].size(); i++) {
			rocketStage.engines.pushBack(LoadManagerMaps::engine[(data["engines"][toS(i)]).split('(')[0]]);
			rocketStage.engines[i].localID = i;
			rocketStage.engines[i].transform.translation = returnVector3(returnArgs(data["engines"][toS(i)])[0]);
		}
		for (auto i = 0; i < data["reactionthrusters"].size(); i++) {
			rocketStage.reactionThrusters.pushBack(LoadManagerMaps::engine[(data["reactionthrusters"][toS(i)]).split('(')[0]]);
			rocketStage.reactionThrusters[i].localID = i;
			rocketStage.reactionThrusters[i].transform.translation = returnVector3(returnArgs(data["reactionthrusters"][toS(i)])[0]);
		}
		for (auto i = 0; i < data["fueltanks"].size(); i++) {
			rocketStage.fuelTanks.pushBack(LoadManagerMaps::fuelTank[(data["fueltanks"][toS(i)]).split('(')[0]]);
			rocketStage.fuelTanks[i].localID = i;
			rocketStage.fuelTanks[i].transform.translation = returnVector3(returnArgs(data["fueltanks"][toS(i)])[0]);
		}

		return rocketStage;
	}

	void validatePlanetFileVariables(std::unordered_map<String, std::unordered_map<String, String>> map) {

	}

	bool loadPlanet(String planetFile, PhysicsPlanet::Builder& physicsPlanet, FixedOrbitPlanet::Builder& fixedOrbitPlanet) {
		std::ifstream file(planetFile.cstr());
		auto data = loadBatches(file);
		validatePlanetFileVariables(data);

		Vector<Obstruction> obstructions;

		if (data["setup"]["fixedorbit"] == "true") {
			fixedOrbitPlanet.name = planetFile.split('\\')[planetFile.split('\\').size() - 1].split('.')[0];
			fixedOrbitPlanet.mass = STold(data["setup"]["mass"]);
			fixedOrbitPlanet.radius = STold(data["setup"]["radius"]);
			fixedOrbitPlanet.model = LoadManagerMaps::model[data["setup"]["model"]];
			return true;
		}
		physicsPlanet.name = planetFile.split('\\')[planetFile.split('\\').size() - 1].split('.')[0];
		physicsPlanet.mass = STold(data["setup"]["mass"]);
		physicsPlanet.radius = STold(data["setup"]["radius"]);
		physicsPlanet.model = LoadManagerMaps::model[data["setup"]["model"]];
		physicsPlanet.transform.translation = returnVector3(data["setup"]["pos"]);
		return false;
	}

	void validateRocketVariables(std::unordered_map<String, std::unordered_map<String, String>> map, bool& onPlanetStart) {
		String misingBatches;
		validationLayer(map, "setup", misingBatches);
		validationLayer(map, "stage", misingBatches);
		if (misingBatches != "")
			throw error("There is mising batches in the file:\n" + misingBatches + "Check for spelling errors.", exitCodes::badUserBehavior);

		String misingValues;
		validationLayer(map["setup"], "orientation", misingValues);
		validationLayer(map["setup"], "pos", misingValues);
		validationLayer(map["setup"], "vel", misingValues);
		validationLayer(map["setup"], "acc", misingValues);
		if (misingValues != "")
			throw error("There is mising values in the file:\n" + misingValues + "Check for spelling errors.", exitCodes::badUserBehavior);

		String notLoadedObjects;
		validateThatObjectshasBeenLoaded(map["stage"], LoadManagerMaps::rocketStage, notLoadedObjects);
		if(notLoadedObjects != "")
			throw error("Therese objects are mising: \n" + notLoadedObjects + "Check for spelling errors", exitCodes::badUserBehavior);
	}

	Rocket::Builder loadRocket(String rocketFile) {
		std::ifstream file(rocketFile.cstr());
		auto data = loadBatches(file);
		bool onPlanetStart;
		validateRocketVariables(data, onPlanetStart);
		file.close();

		Rocket::Builder rocket;
		rocket.name = rocketFile.split('\\')[rocketFile.split('\\').size() - 1].split('.')[0];
		rocket.stages = Vector<RocketStage::Builder>(data["stage"].size());
		RocketStage::Builder rocketStage;
		for (auto i = 0; i < data["stage"].size(); i++) {
			rocketStage = LoadManagerMaps::rocketStage[(data["stage"][toS(i)]).split('(')[0]];
			rocketStage.localID = i;
			rocketStage.transform.translation = returnVector3(returnArgs(data["stage"][toS(i)])[0]);
			rocket.stages.pushBack(rocketStage);
		}
		rocket.transform.translation = returnVector3(data["setup"]["pos"]);
		rocket.transform.rotation = returnQuaternion(data["setup"]["orientation"]);
		rocket.velosity = returnVector3(data["setup"]["vel"]);
		rocket.accseleration = returnVector3(data["setup"]["acc"]);
		return rocket;
	}

	void validateSettingVariables(std::unordered_map<String, String> map) {
		String misingValues;
		validationLayer(map, "gravity", misingValues);
		validationLayer(map, "aerodynamic", misingValues);
		validationLayer(map, "randomnes", misingValues);
		validationLayer(map, "edge detection", misingValues);
		validationLayer(map, "point approximation of mesh per m2", misingValues);
		validationLayer(map, "delta time", misingValues);
		validationLayer(map, "used fuel types", misingValues);
		if (misingValues != "")
			throw error("There is mising values in the file:\n" + misingValues + "Check for spelling errors.", exitCodes::badUserBehavior);
	}

	void validateAllLoadedObjects(std::unordered_map<String, String> settings) {
		Vector<String> validFuelTypes = settings["usedfueltypes"].split(',');
		for (auto& [key, val] : LoadManagerMaps::engine) {
			for (auto i = 0; i < val.fuelPerSecond.fuelTypes().size(); i++)
				if (validFuelTypes.linearSearch(val.fuelPerSecond.fuelTypes()[i]) == -1)
					throw error("Rocket engine: " + key + ". uses a fuel that is not valid( " + val.fuelPerSecond.fuelTypes()[i] + " ). check the speling", exitCodes::badUserBehavior);
		}
		for (auto& [key, val] : LoadManagerMaps::fuelTank) {
			if (validFuelTypes.linearSearch(val.fuelType) == -1)
				throw error("Rocket fuel tank: " + key + ". uses a fuel that is not valid( " + val.fuelType + " ). check the speling", exitCodes::badUserBehavior);
		}
	}

	void loadSettings(String settingsFile) {
		std::ifstream file(settingsFile.cstr());
		auto settings = loadVariablesAndValuesInToMap(file);

		try {
			validateSettingVariables(settings);
		}
		catch (error& e) {
			e.what = "Error in file: " + settingsFile + ":\n" + e.what;
			throw e;
		}
		if (settings["gravity"] == "newton")
			flags::gravitySimulation = flags::newton;
		if (settings["gravity"] == "generalrelativity")
			flags::gravitySimulation = flags::generalRelativity;

		if (settings["aerodynamic"] == "simple")
			flags::gravitySimulation = flags::simple;
		if (settings["aerodynamic"] == "cfd")
			flags::gravitySimulation = flags::CFD;

		flags::random = (settings["random"] == "true") ? true : false;
		options::randomSeed = STou(settings["seed"]);
		timeObjects::dt = STold(settings["deltatime"]);
		options::edgeDetectionIterations = STold(settings["edgedetection"]);

		validateAllLoadedObjects(settings);
	}

	void loadingFinishingTouches()
	{
		Vector<std::shared_ptr<TransformComponent3D>> transformComponents;
		for (auto& rocket : objectLists::rockets) {
			transformComponents.pushBack(rocket->transform());
			for (auto& rocketStage : rocket->stages()) {
				rocketStage->getTransform()->addParentTransform(transformComponents);
				transformComponents.pushBack(rocketStage->getTransform());
				for (auto& engine : rocketStage->getEngines())
					engine->getTransform()->addParentTransform(transformComponents);
				for (auto& reactionThruster : rocketStage->getReactionThrusters())
					reactionThruster->getTransform()->addParentTransform(transformComponents);
				for (auto& fuelTank : rocketStage->getFuelTanks())
					fuelTank->getTransform()->addParentTransform(transformComponents);
				transformComponents.popBack();
			}
			transformComponents.popBack();
		}
	}

	void createObjects() {

	}

	std::unordered_map<String, String> loadVariablesAndValuesInToMap(std::ifstream& file) {
		String line;
		std::unordered_map<String, String> map;
		while (getline(file, line) && (line.contains("}") == line.contains("{"))) {
			if (line.length() == 0)
				continue;
			Vector<String> vandv = returnVariableAndValue(line);
			map[vandv[0]] = vandv[1];
		}
		return map;
	}

	std::unordered_map<String, std::unordered_map<String, String>> loadBatches(std::ifstream& file) {
		String line;
		std::unordered_map<String, std::unordered_map<String, String>> batches;
		while (getline(file, line)) {
			if (line.contains("{")) {
				line.remove('{');
				line.remove(' ');
				batches[lower(line)] = loadVariablesAndValuesInToMap(file);
			}
		}
		return batches;
	}
} //fileSystem