#include "logging.hpp"

#include <filesystem>

#include "WriteFile.hpp"

#include "fileSystem.hpp"

namespace fs = std::filesystem;
namespace fileSystem {
	void loggingStartup() {
		fs::remove_all(objects::runFolder.cstr());
		fs::create_directories(objects::runFolder.cstr());
		fs::create_directories((objects::runFolder + "config/").cstr());


		fs::copy(objects::simulationFolder.cstr(), (objects::runFolder + "config/").cstr(), fs::copy_options::update_existing | fs::copy_options::recursive);

		fs::create_directories((objects::runFolder + "rocket/").cstr());
		fs::create_directories((objects::runFolder + "rocket/rocketStage/").cstr());
		fs::create_directories((objects::runFolder + "rocket/rocketStage/engine/").cstr());
		fs::create_directories((objects::runFolder + "rocket/rocketStage/fuelTank/").cstr());
		fs::create_directories((objects::runFolder + "planet/").cstr());

		for (const auto rocket : objectLists::rockets)
			createLoggingFilesForNewRocket(*rocket);
		for (const auto planet : objectLists::fixedOrbitPlanets)
			createNewPlanetLoggingFile(planet->ID());
		for (const auto planet : objectLists::physicsPlanets)
			createNewPlanetLoggingFile(planet->ID());
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

#ifndef _DEBUG
		objects::errorLogFile.close();
#endif
	}

	void createGeneralRunInfo() {
		std::ofstream file((objects::runFolder + "GeneralRunInfo.txt").cstr());
		file << "randomSeed=" << toS(options::randomSeed) << std::endl;
		file << "totalTime=" << toS(timeObjects::currentTime);
		file.close();
	}

	void createLoggingFilesForNewRocket(const Rocket& rocket) {
		createNewRocketLoggingFile(rocket.ID());
		for (const auto& rocketStage : rocket.stages()) {
			createNewRocketStageLoggingFile(rocket.ID() + toS(rocketStage.ID()));
			for (const auto& engine : rocketStage.engineIDs())
				createNewEngineLoggingFile(rocket.ID() + toS(rocketStage.ID()) + toS(engine));
			for (const auto& fuelTank : rocketStage.fuelTankIDs())
				createNewFuelTankLoggingFile(rocket.ID() + toS(rocketStage.ID()) + toS(fuelTank));
		}
	}

	void createNewRocketLoggingFile(const String& ID)
	{
		objects::rocketFiles.insert({ ID,  WriteFile<ld>(objects::runFolder + "rocket/" + ID + ".db") });
		objects::rocketFiles[ID].addcolumns({ "time",
			"pos.x", "pos.y", "pos.z",
			"vel.x", "vel.y", "vel.z",
			"acc.x", "acc.y", "acc.z",
			"orientation.w", "orientation.x", "orientation.y", "orientation.z",
			"rotationVel.w", "rotationVel.x", "rotationVel.y", "rotationVel.z",
			"rotationAcc.w", "rotationAcc.x", "rotationAcc.y", "rotationAcc.z",
			"RCS"
			});
	}

	void createNewRocketStageLoggingFile(const String& ID)
	{
		objects::rocketStageFiles.insert({ ID, WriteFile<ld>(objects::runFolder + "rocket/rocketStage/" + ID + ".db") });
		objects::rocketStageFiles[ID].addcolumns({ "time", "staged"});
	}

	void createNewEngineLoggingFile(const String& ID)
	{
		objects::engineFiles.insert({ ID, WriteFile<ld>(objects::runFolder + "rocket/rocketStage/engine/" + ID + ".db") });
		objects::engineFiles[ID].addcolumns({ "time",
			"orientation.w", "orientation.x", "orientation.y", "orientation.z",
			"active", "thrustPercent"
			});
	}

	void createNewFuelTankLoggingFile(const String& ID)
	{
		objects::fuelTankFiles.insert({ ID, WriteFile<ld>(objects::runFolder + "rocket/rocketStage/fuelTank/" + ID + ".db") });
		objects::fuelTankFiles[ID].addcolumns({ "time",
			"fuelMass"
			});
	}

	void createNewPlanetLoggingFile(const String& ID)
	{
		objects::planetFiles.insert({ ID, WriteFile<ld>(objects::runFolder + "planet/" + ID + ".db") });
		objects::planetFiles[ID].addcolumns({ "time",
			"pos.x", "pos.y", "pos.z",
			"vel.x", "vel.y", "vel.z",
			});
	}

	/*
	endres slik at dem ikke lagres hver gang men mår dem forandre seg for rocketStage, fueltank og engine
	kansje også slik at fixedOrbitPlanet ikke treng å bli lagra siden den hlold sæ stabil på en bane
	*/

	void loggCurrentState() {
		for (const auto& rocket : objectLists::rockets) {
			loggRocket(*rocket);
			for (const auto& rocketStage : rocket->stages()) {
				loggRocketStage(rocketStage, rocket->ID() + toS(rocketStage.ID()));
				for (const auto& engine : rocketStage.engines())
					loggEngine(engine, rocket->ID() + toS(rocketStage.ID()) + toS(engine.ID()));
				for (const auto& fuelTank : rocketStage.fuelTanks())
					loggFuelTank(fuelTank, rocket->ID() + toS(rocketStage.ID()) + toS(fuelTank.ID()));
			}
		}
		for (const auto planet : objectLists::physicsPlanets)
			loggPlanet(*planet);
		for (const auto planet : objectLists::fixedOrbitPlanets)
			loggPlanet(*planet);

		timeObjects::dtInstancesSinceLastLogging = 0;
	}

	void loggRocket(const Rocket& rocket) {
		objects::rocketFiles[rocket.ID()].addData({ timeObjects::currentTime,
			rocket.pos().x, rocket.pos().y, rocket.pos().z,
			rocket.vel().x, rocket.vel().y, rocket.vel().z,
			rocket.acc().x, rocket.acc().y, rocket.acc().z,
			rocket.orientation().w, rocket.orientation().x, rocket.orientation().y, rocket.orientation().z,
			rocket.rotationVel().w, rocket.rotationVel().x, rocket.rotationVel().y, rocket.rotationVel().z,
			rocket.rotationAcc().w, rocket.rotationAcc().x, rocket.rotationAcc().y, rocket.rotationAcc().z,
			(rocket.RCS()) ? 1.0l : 0.0l
			});
	}

	void loggRocketStage(const RocketStage& rocketStage, const String& globalID) {
		objects::rocketStageFiles[globalID].addData({ timeObjects::currentTime,
			});
	}

	void loggEngine(const Engine& engine, const String& globalID) {
		objects::engineFiles[globalID].addData({ timeObjects::currentTime,
			engine.orientation().w, engine.orientation().x, engine.orientation().y, engine.orientation().z,
			(engine.active()) ? 1.0l : 0.0l, engine.thrustPercent()
			});
	}

	void loggFuelTank(const FuelTank& fuelTank, const String& globalID) {
		objects::fuelTankFiles[globalID].addData({ timeObjects::currentTime,
			fuelTank.fuelMass()
			});
	}

	void loggPlanet(const PhysicsPlanet& planet) {
		objects::planetFiles[planet.ID()].addData({ timeObjects::currentTime,
			planet.pos().x, planet.pos().y, planet.pos().z,
			planet.vel().x, planet.vel().y, planet.vel().z
			});
	}

	void loggPlanet(const FixedOrbitPlanet& planet) {
		objects::planetFiles[planet.ID()].addData({ timeObjects::currentTime,
			planet.pos().x, planet.pos().y, planet.pos().z,
			planet.vel().x, planet.vel().y, planet.vel().z,
			});
	}
}