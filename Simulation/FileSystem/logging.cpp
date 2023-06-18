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

		for (const auto rocket : objectLists::rockets) {
			objects::rocketFiles.insert({ rocket->ID(),  WriteFile<ld>(objects::runFolder + "rocket/" + rocket->ID() + ".db")});
			objects::rocketFiles[rocket->ID()].addcolumns({ "time" });
			for (const auto& rocketStage : rocket->stages()) {
				objects::rocketStageFiles.insert({ rocket->ID() + toS(rocketStage.ID()), WriteFile<ld>(objects::runFolder + "rocket/rocketStage/" + rocket->ID() + toS(rocketStage.ID()) + ".db") });
				objects::rocketStageFiles[rocket->ID() + toS(rocketStage.ID())].addcolumns({ "time" });
				for (const auto& engine : rocketStage.engineIDs()) {
					objects::engineFiles.insert({ rocket->ID() + toS(rocketStage.ID()) + toS(engine), WriteFile<ld>(objects::runFolder + "rocket/rocketStage/engine/" + rocket->ID() + toS(rocketStage.ID()) + toS(engine) + ".db") });
					objects::engineFiles[rocket->ID() + toS(rocketStage.ID()) + toS(engine)].addcolumns({ "time" });
				}
				for (const auto& fuelTank : rocketStage.fuelTankIDs()) {
					objects::fuelTankFiles.insert({ rocket->ID() + toS(rocketStage.ID()) + toS(fuelTank), WriteFile<ld>(objects::runFolder + "rocket/rocketStage/fuelTank/" + rocket->ID() + toS(rocketStage.ID()) + toS(fuelTank) + ".db") });
					objects::fuelTankFiles[rocket->ID() + toS(rocketStage.ID()) + toS(fuelTank)].addcolumns({ "time" });
				}
			}
		}
		for (const auto planet : objectLists::fixedOrbitPlanets) {
			objects::planetFiles.insert({ planet->ID(), WriteFile<ld>(objects::runFolder + "planet/" + planet->ID() + ".db") });
			objects::planetFiles[planet->ID()].addcolumns({ "time" });
		}
		for (const auto planet : objectLists::physicsPlanets) {
			objects::planetFiles.insert({ planet->ID(), WriteFile<ld>(objects::runFolder + "planet/" + planet->ID() + ".db") });
			objects::planetFiles[planet->ID()].addcolumns({ "time" });
		}
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
			rocket.mass(), (rocket.RCS()) ? 1.0 : 0,
			});
	}

	void loggRocketStage(const RocketStage& rocketStage, const String& globalID) {
		objects::rocketStageFiles[globalID].addData({ timeObjects::currentTime,
			});
	}

	void loggEngine(const Engine& engine, const String& globalID) {
		objects::engineFiles[globalID].addData({ timeObjects::currentTime,
			});
	}

	void loggFuelTank(const FuelTank& fuelTank, const String& globalID) {
		objects::fuelTankFiles[globalID].addData({ timeObjects::currentTime,
			});
	}

	void loggPlanet(const PhysicsPlanet& planet) {
		objects::planetFiles[planet.ID()].addData({ timeObjects::currentTime,
			});
	}

	void loggPlanet(const FixedOrbitPlanet& planet) {
		objects::planetFiles[planet.ID()].addData({ timeObjects::currentTime,
			});
	}
}