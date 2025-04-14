#include "logging.hpp"

#include <filesystem>

#include "WriteFile.hpp"

#include "fileSystem.hpp"
#include "../ObjectRenderingCashing.hpp"
#include "../helpers/simulationObjects.hpp"

namespace fs = std::filesystem;
namespace fileSystem {
	void loggingStartup() {
		fs::remove_all(objects::runFolder.cstr());
		fs::create_directories(objects::runFolder.cstr());
		fs::create_directories((objects::runFolder + "config\\").cstr());


		fs::copy(objects::simulationFolder.cstr(), (objects::runFolder + "config\\").cstr(), fs::copy_options::update_existing | fs::copy_options::recursive);

		fs::create_directories((objects::runFolder + "rocket\\").cstr());
		fs::create_directories((objects::runFolder + "rocket\\rocketStage\\").cstr());
		fs::create_directories((objects::runFolder + "rocket\\rocketStage\\engine\\").cstr());
		fs::create_directories((objects::runFolder + "rocket\\rocketStage\\fuelTank\\").cstr());
		fs::create_directories((objects::runFolder + "planet\\").cstr());

		for (const auto rocket : objectLists::rockets)
			createLoggingFilesForNewRocket(rocket);
		for (const auto planet : objectLists::fixedOrbitPlanets)
			createNewPlanetLoggingFile(planet->getID());
		for (const auto planet : objectLists::physicsPlanets)
			createNewPlanetLoggingFile(planet->getID());
		loggCurrentState();
	}

	void loggingEnd() {
		createGeneralRunInfo();
		loggCurrentState();

		for (auto& [key, val] : objects::engineFiles)
			val.close();
		for (auto& [key, val] : objects::fuelTankFiles)
			val.close();
		for (auto& [key, val] : objects::rocketStageFiles)
			val.close();
		for (auto& [key, val] : objects::rocketFiles)
			val.close();
		for (auto& [key, val] : objects::planetFiles)
			val.close();

		objects::errorLogFile.close();
	}

	void createGeneralRunInfo() {
		std::ofstream file((objects::runFolder + "GeneralRunInfo.txt").cstr());
		file << "randomSeed=" << toS(options::randomSeed) << std::endl;
		file << "totalTime=" << toS(timeObjects::currentTime);
		file.close();
	}

	void loggError(Error error)
	{
		objects::errorLogFile << "Function: " << error._function << " in file: " << error._file << " on line: " << error._line << ", \n"
			<< "what: " << error._what << ",\n"
			<< "code: " << Error::exitCodesNames.at(error._code) << ",\n"
			<< "recovery type: " << Error::recoveryTypeName.at(error._recoveryType) << ",\n"
			<< "error accured during recovery attempt: " << (error._duringRecoveryAttempt) ? "true" : "false";
	}

	void createLoggingFilesForNewRocket(const std::shared_ptr<Rocket> rocket) {
		createNewRocketLoggingFile(rocket->getID());
		for (auto& rocketStage : rocket->stages()) {
			createNewRocketStageLoggingFile(rocketStage->getID());
			for (const auto& engine : rocketStage->getEngineIDs())
				createNewEngineLoggingFile(engine);
			for (const auto& fuelTank : rocketStage->getFuelTankIDs())
				createNewFuelTankLoggingFile(fuelTank);
		}
	}

	void createNewRocketLoggingFile(const IDview& id)
	{
		objects::rocketFiles.insert({ id.getID(),  WriteFile<ld>(objects::runFolder + "rocket\\" + id.getName() + "." + toS(id.getLocalID()) + ".db")});
		objects::rocketFiles[id.getID()].addcolumns({"time",
			"pos.x", "pos.y", "pos.z",
			"vel.x", "vel.y", "vel.z",
			"acc.x", "acc.y", "acc.z",
			"orientation.w", "orientation.x", "orientation.y", "orientation.z",
			"rotationVel.w", "rotationVel.x", "rotationVel.y", "rotationVel.z",
			"rotationAcc.w", "rotationAcc.x", "rotationAcc.y", "rotationAcc.z",
			"RCS"
			});
	}

	void createNewRocketStageLoggingFile(const IDview& id)
	{
		objects::rocketStageFiles.insert({ id.getID(), WriteFile<ld>(objects::runFolder + "rocket\\rocketStage\\" + id.getName() + "." + toS(id.getLocalID()) + ".db")});
		objects::rocketStageFiles[id.getID()].addcolumns({ "time", "staged"});
	}

	void createNewEngineLoggingFile(const IDview& id)
	{
		objects::engineFiles.insert({ id.getID(), WriteFile<ld>(objects::runFolder + "rocket\\rocketStage\\engine\\" + id.getName() + "." + toS(id.getLocalID()) + ".db") });
		objects::engineFiles[id.getID()].addcolumns({ "time",
			"orientation.w", "orientation.x", "orientation.y", "orientation.z",
			"active", "thrustPercent"
			});
	}

	void createNewFuelTankLoggingFile(const IDview& id)
	{
		objects::fuelTankFiles.insert({ id.getID(), WriteFile<ld>(objects::runFolder + "rocket\\rocketStage\\fuelTank\\" + id.getName() + "." + toS(id.getLocalID()) + ".db") });
		objects::fuelTankFiles[id.getID()].addcolumns({ "time",
			"fuelMass"
			});
	}

	void createNewPlanetLoggingFile(const IDview& id)
	{
		objects::planetFiles.insert({ id.getID(), WriteFile<ld>(objects::runFolder + "planet\\" + id.getName() + "." + toS(id.getLocalID()) + ".db") });
		objects::planetFiles[id.getID()].addcolumns({ "time",
			"pos.x", "pos.y", "pos.z",
			"vel.x", "vel.y", "vel.z",
			});
	}

	/*
	endres slik at dem ikke lagres hver gang men mår dem forandre seg for rocketStage, fueltank og engine
	kansje også slik at fixedOrbitPlanet ikke treng å bli lagra siden den hlold sæ stabil på en bane
	*/

	void loggCurrentState() {
		SimulationTimeCash simCash;
		simCash.time = timeObjects::currentTime;
		for (const auto& rocket : objectLists::rockets) {
			loggRocket(*rocket);
			for (auto& rocketStage : rocket->stages()) {
				loggRocketStage(*rocketStage);
				simCash.objects[rocketStage->getID().getID()] = *rocketStage->getTransform();
				for (auto& engine : rocketStage->getEngines()) {
					loggEngine(*engine);
					simCash.objects[engine->getID().getID()] = *engine->getTransform();
				}
				for (auto& fuelTank : rocketStage->getFuelTanks()) {
					loggFuelTank(*fuelTank);
					simCash.objects[fuelTank->getID().getID()] = *fuelTank->getTransform();
				}
			}
		}
		for (const auto planet : objectLists::physicsPlanets) {
			loggPlanet(*planet);
			simCash.objects[planet->getID().getID()] = *planet->getTransform();
		}
		for (const auto planet : objectLists::fixedOrbitPlanets) {
			loggPlanet(*planet);
		}

		timeObjects::dtInstancesSinceLastLogging = 0;
	}

	void loggRocket(const Rocket& rocket) {
		objects::rocketFiles[rocket.getID().getID()].addData({ timeObjects::currentTime,
			rocket.pos().x, rocket.pos().y, rocket.pos().z,
			rocket.vel().x, rocket.vel().y, rocket.vel().z,
			rocket.acc().x, rocket.acc().y, rocket.acc().z,
			rocket.orientation().w, rocket.orientation().x, rocket.orientation().y, rocket.orientation().z,
			rocket.rotationVel().w, rocket.rotationVel().x, rocket.rotationVel().y, rocket.rotationVel().z,
			rocket.rotationAcc().w, rocket.rotationAcc().x, rocket.rotationAcc().y, rocket.rotationAcc().z,
			(rocket.RCS()) ? 1.0l : 0.0l
			});
	}

	void loggRocketStage(const RocketStage& rocketStage) {
		objects::rocketStageFiles[rocketStage.getID().getID()].addData({ timeObjects::currentTime,
			});
	}

	void loggEngine(const Engine& engine) {
		objects::engineFiles[engine.getID().getID()].addData({timeObjects::currentTime,
			engine.getOrientation().w, engine.getOrientation().x, engine.getOrientation().y, engine.getOrientation().z,
			(engine.active()) ? 1.0l : 0.0l, engine.getThrustPercent()
			});
	}

	void loggFuelTank(const FuelTank& fuelTank) {
		objects::fuelTankFiles[fuelTank.getID().getID()].addData({ timeObjects::currentTime,
			fuelTank.getFuelMass()
			});
	}

	void loggPlanet(const PhysicsPlanet& planet) {
		objects::planetFiles[planet.getID().getID()].addData({ timeObjects::currentTime,
			planet.getPos().x, planet.getPos().y, planet.getPos().z,
			planet.getVel().x, planet.getVel().y, planet.getVel().z
			});
	}

	void loggPlanet(const FixedOrbitPlanet& planet) {
		objects::planetFiles[planet.getID().getID()].addData({ timeObjects::currentTime,
			planet.getPos().x, planet.getPos().y, planet.getPos().z,
			planet.getVel().x, planet.getVel().y, planet.getVel().z,
			});
	}
}