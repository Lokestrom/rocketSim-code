#include "LoadManager.hpp"

#include <fstream>
#include <filesystem>
#include <iostream>

#include <String.hpp>

#include "controles.hpp"
#include "flags.hpp"
#include "Rocket.hpp"
#include "planet.hpp"

namespace fs = std::filesystem;
using namespace fileSystem;

struct error {
	String what;
	errorSeverityLevel level;
};

namespace loadingVar{
	errorSeverityLevel exitLevel;
}

#if debug
	void writeError(error e) {
		std::cout << e.what << "\n";
		if (e.level >= loadingVar::exitLevel)
			std::exit(e.level+1);
	}
#else
	void writeError(error e) {
		fileSystem::errorLogFile << e.what << "\n";
		if (e.level >= loadingVar::exitLevel)
			std::exit(e.level + 1);
	}
#endif

void loadInObjects() {
	simulationFolder += "/Config";
	for (const auto& entry : fs::directory_iterator(toSTD(simulationFolder + "/Mesh")))
		LoadManagerObjects::meshMap[String(entry.path().filename().string())] = loadMesh(entry.path().string());
	for (const auto& entry : fs::directory_iterator(toSTD(simulationFolder + "/Engine"))) {
		bool isReactionThruster;
		Engine engine = loadEngine(entry.path().string(), isReactionThruster);
		if (isReactionThruster)
			LoadManagerObjects::reactionThrusterMap[String(entry.path().filename().string())] = engine;
		else
			LoadManagerObjects::engineMap[String(entry.path().filename().string())] = engine;
	}
	for (const auto& entry : fs::directory_iterator(toSTD(simulationFolder + "/FuelTank")))
		LoadManagerObjects::fuelTankMap[String(entry.path().filename().string())] = loadFuelTank(entry.path().string());
	for (const auto& entry : fs::directory_iterator(toSTD(simulationFolder + "/Planet"))) {
		PhysicsPlanet physicsPlanet;
		FixedOrbitPlanet fixedOrbitPlanet;
		bool isPhysicsPlanet = loadPlanet(entry.path().string(), physicsPlanet, fixedOrbitPlanet);

		if (isPhysicsPlanet)
			LoadManagerObjects::pysicsPlanetMap[String(entry.path().filename().string())] = physicsPlanet;
		else
			LoadManagerObjects::fixedOrbitPlanetMap[String(entry.path().filename().string())] = fixedOrbitPlanet;
	}
	for (const auto& entry : fs::directory_iterator(toSTD(simulationFolder + "/RocketStage")))
		LoadManagerObjects::rocketStageMap[String(entry.path().filename().string())] = loadRocketStage(entry.path().string());
	
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
			shape.meshes.pushBack(ShapeNode(MeshType::Sphere, 
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
			writeError(error("The mesh type: " + type + ". Is not a valid mesh", low));
			
		}
	}
	file.close();
	return shape;
}

bool validationLayer(const std::unordered_map<String, String>& map, const String& variable, String& msg) {
	if (!map.count(toSTD(lower(variable).remove(' ')))) {
		msg += "\t" + variable + "\n";
		return false;
	}
	return true;
}

bool validationLayer(const std::unordered_map<String, std::unordered_map<String, String>>& map, const String& variable, String& msg) {
	if (!map.count(toSTD(lower(variable).remove(' ')))) {
		msg += "\t" + variable + "\n";
		return false;
	}
	return true;
}

template<typename T>
void validateThatObjectshasBeenLoaded(const std::unordered_map<String, String>& map, const std::unordered_map<String, T>& fileMap, String& msg) {
	for (const auto& [key, val] : map) {
		if (!fileMap.count(val))
			msg += "\t" + val + "\n";
	}
}

void validateThatValueIsNotNegative(const std::unordered_map<String, String> map, const String& val) {
	if (STold(map[val]) < 0)
		throw error((val + " can't be negative."), low);
}

void validateEngineVariables(std::unordered_map<String, String> map) {
	String misingValues;
	validationLayer(map, "fuel per second", misingValues);
	validationLayer(map, "mesh", misingValues);
	validationLayer(map, "mass", misingValues);
	validationLayer(map, "exit velosity", misingValues);
	validationLayer(map, "pos", misingValues);
	validationLayer(map, "center of mass", misingValues);
	validationLayer(map, "mount pos", misingValues);
	if (misingValues != "")
		throw error("There is mising values in the Engine file:\n" + misingValues + "Check for spelling errors.", severe);
	if (!LoadManagerObjects::meshMap.count(map["mesh"]))
		throw error("The mesh: " + map["mesh"] + "has not been loaded. Check for spelling errors", severe);

	try {
		validateThatValueIsNotNegative(map, "mass");
		validateThatValueIsNotNegative(map, "exitvelosity");
		validateThatValueIsNotNegative(map, "fuelpersecond");
	}
	catch (error& e) {
		throw e;
	}

	if (!map.count("maxgimblepersecond") && map.count("maxgimble"))
		throw error("there is a value for max gimble but not for max gimble per second. you may have forgoten the max gimble per second variable", low);
	if (map.count("maxgimblepersecond") && !map.count("maxgimble"))
		throw error("there is a value for max gimble per second but not for max gimble. you may have forgoten the max gimble variable", low);

}

Engine loadEngine(String engineFile, bool reactionThruster) {
	std::ifstream file(engineFile.cstr());
	String line;
	std::unordered_map<String, String> map = loadVariablesAndValuesInToMap(file);
	try {
		validateEngineVariables(map);
	}
	catch (error& e) {
		e.what = "Error in file: " + engineFile + ":\n" + e.what;
		writeError(e);
	}
	Fuelmap fuelPerSecond;
	Vector<String> fuelPerSecondArgs = map["fuelpersecond"].split(',');
	for (const auto& i : fuelPerSecondArgs) {
		Vector<String> fuel = i.split(':');
		fuelPerSecond += Fuelmap(fuel[0], STold(fuel[1]));
	}
	file.close();
	Shape mesh = LoadManagerObjects::meshMap[map["mesh"]];
	if (!map.count("maxgimblepersecond") && !map.count("maxgimble"))
		return Engine(0, STold(map["mass"]), STold(map["exitvelosity"]), returnVector3(map["pos"]), returnVector3(map["centerofmass"]), returnVector3(map["mountpos"]), fuelPerSecond, mesh);
	return Engine(0, STold(map["mass"]), STold(map["exitvelosity"]), returnVector3(map["pos"]), returnVector3(map["centerofmass"]), returnVector3(map["mountpos"]), fuelPerSecond, mesh, degToRad(STold(map["maxgimblepersecond"])), degToRad(STold(map["maxgimble"])));
}

void validateFuelTankVariables(std::unordered_map<String, String> map) {
	String misingValues;
	validationLayer(map, "fuel type", misingValues);
	validationLayer(map, "radius", misingValues);
	validationLayer(map, "height", misingValues);
	validationLayer(map, "fuel density", misingValues);
	validationLayer(map, "mass", misingValues);
	if (misingValues != "")
		throw error("There is mising values in the file:\n" + misingValues + "Check for spelling errors.", severe);

	try {
		validateThatValueIsNotNegative(map, "mass");
		validateThatValueIsNotNegative(map, "fueldensity");
		validateThatValueIsNotNegative(map, "radius");
		validateThatValueIsNotNegative(map, "height");
	}
	catch (error& e) {
		throw e;
	}

	if (PI * STold(map["radius"]) * STold(map["radius"]), STold(map["height"]) < STold(map["fuelmass"]) * STold(map["fueldensity"]))
		throw error("There is more mass in the tank than there is volum. Decrease the fuel amont, increase the density or make the fuel tank bigger", low);
}

FuelTank loadFuelTank(String FuelTankFile) {
	std::ifstream file(FuelTankFile.cstr());
	String line;
	std::unordered_map<String, String> map = loadVariablesAndValuesInToMap(file);
	file.close();
	try {
		validateFuelTankVariables(map);
	}
	catch (error& e) {
		e.what = "Error in file: " + FuelTankFile + ":\n" + e.what;
		writeError(e);
	}
	return FuelTank(0, map["fueltype"], STold(map["fuelload"]), STold(map["radius"]), STold(map["height"]), STold(map["fueldensity"]));
}

void validateRocketStageVariables(std::unordered_map<String, std::unordered_map<String, String>> map) {
	String misingBatches;
	validationLayer(map, "setup", misingBatches);
	validationLayer(map, "engines", misingBatches);
	validationLayer(map, "reaction thruster", misingBatches);
	validationLayer(map, "fuel tank", misingBatches);
	if (misingBatches != "")
		throw error("There is mising batches in the file:\n" + misingBatches + "Check for spelling errors.", severe);

	String misingValues;
	validationLayer(map["setup"], "center of mass", misingValues);
	validationLayer(map["setup"], "mesh", misingValues);
	validationLayer(map["setup"], "dry mass", misingValues);
	if (misingValues != "")
		throw error("There is mising values in the file:\n" + misingValues + "Check for spelling errors.", severe);
	if (!LoadManagerObjects::meshMap.count(map["setup"]["mesh"]))
		throw error("The mesh: " + map["setup"]["mesh"] + "has not been loaded. Check for spelling errors", severe);

	try {
		validateThatValueIsNotNegative(map["setup"], "drymass");
	}
	catch (error& e) {
		throw e;
	}

	String notLoadedObjects;
	validateThatObjectshasBeenLoaded(map["engine"], LoadManagerObjects::engineMap, notLoadedObjects);
	validateThatObjectshasBeenLoaded(map["reactionthruster"], LoadManagerObjects::reactionThrusterMap, notLoadedObjects);
	validateThatObjectshasBeenLoaded(map["fueltank"], LoadManagerObjects::fuelTankMap, notLoadedObjects);
	
	throw error("Therese objects are mising: \n" + notLoadedObjects + "Check for spelling errors", severe);
}

RocketStage loadRocketStage(String rocketStageFile) {
	std::ifstream file(rocketStageFile.cstr());
	auto data = loadBatches(file);
	validateRocketStageVariables(data);

	Shape mesh = LoadManagerObjects::meshMap[data["setup"]["mesh"]];
	
	Vector<Engine> engines;
	for (auto i = 0; i < data["engine"].size(); i++) {
		engines.pushBack(LoadManagerObjects::engineMap[(data["engine"][toS(i)]).split('(')[0]]);
		engines[i].setID(i);
		engines[i].setPos(returnVector3(returnArgs(data["engine"][toS(i)])[0]));
	}
	Vector<ReactionThruster> reactionThrusters;
	for (auto i = 0; i < data["reactionthruster"].size(); i++) {
		reactionThrusters.pushBack(LoadManagerObjects::reactionThrusterMap[(data["reactionthruster"][toS(i)]).split('(')[0]]);
		reactionThrusters[i].setID(i);
		reactionThrusters[i].setPos(returnVector3(returnArgs(data["reactionthruster"][toS(i)])[0]));
	}
	Vector<FuelTank> fuelTanks;
	for (auto i = 0; i < data["fueltank"].size(); i++) {
		fuelTanks.pushBack(LoadManagerObjects::fuelTankMap[(data["fueltank"][toS(i)]).split('(')[0]]);
		fuelTanks[i].setID(i);
		fuelTanks[i].setPos(returnVector3(returnArgs(data["fueltank"][toS(i)])[0]));
	}

	return RocketStage(0, { 0,0,0 }, STold(data["setup"]["drymass"]), returnVector3(data["setup"]["centerofmass"]), engines, reactionThrusters, fuelTanks, mesh);
}

void validatePlanetFileVariables(std::unordered_map<String, std::unordered_map<String, String>> map) {

}

bool loadPlanet(String planetFile, PhysicsPlanet& physicsPlanet, FixedOrbitPlanet& fixedOrbitPlanet) {
	std::ifstream file(planetFile.cstr());
	auto data = loadBatches(file);
	validatePlanetFileVariables(data);

	Vector<Obstruction> obstructions;
}

void validateRocketVariables(std::unordered_map<String, std::unordered_map<String, String>> map, bool& onPlanetStart) {
	String misingBatches;
	validationLayer(map, "setup", misingBatches);
	validationLayer(map, "stage", misingBatches);
	if (misingBatches != "")
		throw error("There is mising batches in the file:\n" + misingBatches + "Check for spelling errors.", severe);

	String misingValues;
	validationLayer(map["setup"], "orientation", misingValues);
	validationLayer(map["setup"], "mesh", misingValues);
	validationLayer(map["setup"], "dry mass", misingValues);
	if (misingValues != "")
		throw error("There is mising values in the file:\n" + misingValues + "Check for spelling errors.", severe);

	String notLoadedObjects;
	validateThatObjectshasBeenLoaded(map["stage"], LoadManagerObjects::rocketStageMap, notLoadedObjects);

	throw error("Therese objects are mising: \n" + notLoadedObjects + "Check for spelling errors", severe);
}

Rocket loadRocket(String rocketFile) {
	std::ifstream file(rocketFile.cstr());
	auto data = loadBatches(file);
	bool onPlanetStart;
	validateRocketVariables(data, onPlanetStart);

	Vector<RocketStage> rocketStages;
	for (auto i = 0; i < data["stage"].size(); i++) {
		rocketStages.pushBack(LoadManagerObjects::rocketStageMap[(data["stage"][toS(i)]).split('(')[0]]);
		rocketStages[i].setID(i);
		rocketStages[i].setPos(returnVector3(returnArgs(data["stage"][toS(i)])[0]));
	}
	Vector3 pos, vel, acc;
	Quaternion rotation;

	return Rocket(rocketFile.split('.')[0], pos, vel, acc, rotation, rocketStages);
}

void validateSettingVariables(std::unordered_map<String, String> map) {
	String misingValues;
	validationLayer(map, "gravity", misingValues);
	validationLayer(map, "aerodynamic", misingValues);
	validationLayer(map, "randomnes", misingValues);
	validationLayer(map, "edge detection", misingValues);
	validationLayer(map, "Point approximation of mesh per m2", misingValues);
	validationLayer(map, "delta time", misingValues);
	validationLayer(map, "used fuel types", misingValues);
	if(misingValues != "")
		throw error("There is mising values in the file:\n" + misingValues + "Check for spelling errors.", severe);
}

bool validateAllLoadedObjects(std::unordered_map<String, String> settings) {
	Vector<String> validFuelTypes = settings["usedfueltypes"].split(',');
	for (auto& [key, val] : LoadManagerObjects::engineMap) {
		for (auto i = 0; i < val.fuelTypes().size(); i++)
			if (validFuelTypes.linearSearch(val.fuelTypes()[i]) == -1)
				throw InvalidArgument(("Rocket engine: " + key + ". uses a fuel that is not valid( " + val.fuelTypes()[i] + " ). check the speling").cstr());
	}
	for (auto& [key, val] : LoadManagerObjects::reactionThrusterMap) {
		for (auto i = 0; i < val.fuelTypes().size(); i++)
			if (validFuelTypes.linearSearch(val.fuelTypes()[i]) == -1)
				throw InvalidArgument(("Rocket reaction thruster: " + key + ". uses a fuel that is not valid( " + val.fuelTypes()[i] + " ). check the speling").cstr());
	}
	for (auto& [key, val] : LoadManagerObjects::fuelTankMap) {
		if (validFuelTypes.linearSearch(val.fuelType()) == -1)
			throw InvalidArgument(("Rocket fuel tank: " + key + ". uses a fuel that is not valid( " + val.fuelType() + " ). check the speling").cstr());
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
		writeError(e);
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
	objects::dt = STold(settings["deltatime"]);
	options::edgeDetectionIterations = STold(settings["edgedetection"]);

	validateAllLoadedObjects(settings);
}

std::unordered_map<String, String> loadVariablesAndValuesInToMap(std::ifstream& file) {
	String line;
	std::unordered_map<String, String> map;
	while (getline(file, line) && !line.contains("}")) {
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
		if (line.contains("{"))
			batches[line.remove('{')] = loadVariablesAndValuesInToMap(file);
	}
	return batches;
}

Vector<String> returnVariableAndValue(String line) {
	line.remove(' ');
	Vector<String> ans = line.split('=');
	if (ans.size() > 2)
		throw error("Too many arguments. Not alowed", mid);
	ans[0].lower();
	ans[1].lower();
	return ans;
}

Vector<String> returnArgs(String args) {
	args = args.remove(' ');
	args.pop(0, args.linearSearch('('));
	args.pop(args.linearSearchR(')'), args.length()-1);
	Vector<String> argv = args.split(',');
	for (auto i = 0; i < argv.size(); i++) {
		if (argv[i].contains("{")) {
			String newArg = "";
			auto j = i;
			for (; !argv[j].contains("}"); j++) {
				newArg += argv[j] + ",";
			}
			newArg += argv[j];
			argv.pop(i, j+1);
			argv.insert(i, newArg);
		}		
	}
	return argv;
}

Vector3 returnVector3(String arg) {
	if (arg.split('{')[0].length() != arg.length()-1) {
		throw error("The arg is labeld with : " + arg.split('{')[0] + "may not be a vector3", low);
	}
	arg.popBack();
	arg.pop(0);
	arg = arg.remove(' ');
	return { STold(arg.split(',')[0]), STold(arg.split(',')[1]), STold(arg.split(',')[2]) };
}

Quaternion returnQuaternion(String arg) {
	if (arg.split('{')[0] != "Quat") {
		throw error("The arg is not labeld a Quaternion. The arg may not be a quaternion", low);
	}
	arg.popBack();
	arg.pop(0);
	arg = arg.remove(' ');
	return toQuaternion({ degToRad(STold(arg.split(',')[0])), degToRad(STold(arg.split(',')[1])), degToRad(STold(arg.split(',')[2])) });
}