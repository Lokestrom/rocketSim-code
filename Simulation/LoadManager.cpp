#include "LoadManager.hpp"

#include <fstream>
#include <filesystem>

#include <String.hpp>

#include "controles.hpp"
#include "flags.hpp"
#include "Rocket.hpp"
#include "planet.hpp"

namespace fs = std::filesystem;
using namespace fileSystem;

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
		if (type == "sphere") {
			shape.meshes.pushBack(ShapeNode(0, );
		}
		else if (type == "box") {
			shape.meshes.pushBack(ShapeNode(1, );
		}
		else if (type == "sylinder") {
			shape.meshes.pushBack(ShapeNode(2, );
		}
		else if (type == "nosecone") {
			shape.meshes.pushBack(ShapeNode(3, );
		}
		else {
			file.close();
			throw InvalidArgument(("The mesh type: " + type + ". Is not a valid mesh").cstr());
		}
	}
	file.close();
	return shape;
}

void validationLayer(const std::unordered_map<String, String>& map, const String& variable, String& msg) {
	if (!map.count(toSTD(lower(variable).remove(' ')))) {
		msg += "\t" + variable + "\n";
	}
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
		throw InvalidArgument(("There is mising values in the Engine file:\n" + misingValues + "Check for spelling errors.").cstr());

	if (!map.count("maxgimblepersecond") && map.count("maxgimble"))
		throw InvalidArgument("there is a value for max gimble but not for max gimble per second. you may have forgoten the max gimble per second variable");
	if (map.count("maxgimblepersecond") && !map.count("maxgimble"))
		throw InvalidArgument("there is a value for max gimble per second but not for max gimble. you may have forgoten the max gimble variable");

}

Engine loadEngine(String engineFile, bool reactionThruster) {
	std::ifstream file(engineFile.cstr());
	String line;
	std::unordered_map<String, String> map = loadVariablesAndValuesInToMap(file);
	validateEngineVariables(map);

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

}

FuelTank loadFuelTank(String FuelTankFile) {
	std::ifstream file(FuelTankFile.cstr());
	String line;
	std::unordered_map<String, String> map = loadVariablesAndValuesInToMap(file);
	file.close();
	validateFuelTankVariables(map);
	return FuelTank(0, map["fueltype"], STold(map["fuelload"]), STold(map["radius"]), STold(map["height"]), STold(map["fueldensity"]));
}

void validateRocketStageVariables(std::unordered_map<String, std::unordered_map<String, String>> map) {

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

	return RocketStage("", STold(data["setup"]["drymass"]), { 0,0,0 }, returnVector3(data["setup"]["centerofmass"]), engines, reactionThrusters, fuelTanks, mesh);
}

void validatePlanetFileVariables(std::unordered_map<String, std::unordered_map<String, String>> map) {

}

bool loadPlanet(String planetFile, PhysicsPlanet& physicsPlanet, FixedOrbitPlanet& fixedOrbitPlanet) {
	std::ifstream file(planetFile.cstr());
	auto data = loadBatches(file);
	validatePlanetFileVariables(data);

	Vector<Obstruction> obstructions;
}

void validateRocketVariables(std::unordered_map<String, std::unordered_map<String, String>> map) {

}

Rocket loadRocket(String rocketFile) {
	std::ifstream file(rocketFile.cstr());
	auto data = loadBatches(file);
	validateRocketVariables(data);

	Vector<RocketStage> rocketStages;
	for (auto i = 0; i < data["stage"].size(); i++) {
		rocketStages.pushBack(LoadManagerObjects::rocketStageMap[(data["stage"][toS(i)]).split('(')[0]]);
		rocketStages[i].setID(i);
		rocketStages[i].setPos(returnVector3(returnArgs(data["stage"][toS(i)])[0]));
	}
	if()
	return Rocket()
	
}

void validateSettingVariables(std::unordered_map<String, String> map) {

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

	validateSettingVariables(settings);

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
		throw InvalidArgument("Too many arguments. Not alowed");
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
		throw InvalidArgument(("The arg is labeld with: " + arg.split('{')[0] + "may not be a vector3").cstr());
	}
	arg.popBack();
	arg.pop(0);
	arg = arg.remove(' ');
	return { STold(arg.split(',')[0]), STold(arg.split(',')[1]), STold(arg.split(',')[2]) };
}

Quaternion returnQuaternion(String arg) {
	if (arg.split('{')[0] != "Quat") {
		throw InvalidArgument("The arg is not labeld a Quaternion. The arg may not be a quaternion");
	}
	arg.popBack();
	arg.pop(0);
	arg = arg.remove(' ');
	return toQuaternion({ degToRad(STold(arg.split(',')[0])), degToRad(STold(arg.split(',')[1])), degToRad(STold(arg.split(',')[2])) });
}