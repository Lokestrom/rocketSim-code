#include "logging.hpp"

#include <filesystem>

#include <WriteFile.hpp>

#include "controles.hpp"
#include "Rocket.hpp"
#include "planet.hpp"

namespace fs = std::filesystem;

void loggingStartup() {
	fs::create_directories(fileSystem::runFolder.cstr());

	fs::copy((fileSystem::simulationFolder + "/config/").cstr(), fileSystem::runFolder.cstr());

	fs::create_directories((fileSystem::runFolder + "rocket/").cstr());
	fs::create_directories((fileSystem::runFolder + "rocket/rocketStage/").cstr());
	fs::create_directories((fileSystem::runFolder + "rocket/rocketStage/engine/").cstr());
	fs::create_directories((fileSystem::runFolder + "rocket/rocketStage/fuelTank/").cstr());
	fs::create_directories((fileSystem::runFolder + "planet/").cstr());

	for (const auto& rocket : *objects::rockets) {
		fileSystem::rocketFiles.insert({ rocket.ID(),  WriteFile<ld>(fileSystem::runFolder + "rocket/" + rocket.ID()) });
		fileSystem::rocketFiles[rocket.ID()].addcolumns({"time"});
		for (const auto& rocketStage : rocket.stages()) {
			fileSystem::rocketStageFiles.insert({ rocket.ID() + toS(rocketStage.ID()), WriteFile<ld>(fileSystem::runFolder + "rocket/rocketStage/" + rocket.ID() + toS(rocketStage.ID())) });
			fileSystem::rocketFiles[rocket.ID() + toS(rocketStage.ID())].addcolumns({"time"});
			for (const auto& engine : rocketStage.engineIDs()) {
				fileSystem::rocketStageFiles.insert({ rocket.ID() + toS(rocketStage.ID()) + toS(engine), WriteFile<ld>(fileSystem::runFolder + "rocket/rocketStage/engine/" + rocket.ID() + toS(rocketStage.ID()) + toS(engine)) });
				fileSystem::rocketFiles[rocket.ID() + toS(rocketStage.ID()) + toS(engine)].addcolumns({"time"});
			}
			for (const auto& fuelTank : rocketStage.fuelTankIDs()) {
				fileSystem::rocketStageFiles.insert({ rocket.ID() + toS(rocketStage.ID()) + toS(fuelTank), WriteFile<ld>(fileSystem::runFolder + "rocket/rocketStage/fuelTank/" + rocket.ID() + toS(rocketStage.ID()) + toS(fuelTank)) });
				fileSystem::rocketFiles[rocket.ID() + toS(rocketStage.ID()) + toS(fuelTank)].addcolumns({"time"});
			}
		}
	}
	for (const auto& planet : *objects::fixedOrbitPlanets) {
		fileSystem::planetFiles.insert({ planet.ID(), WriteFile<ld>(fileSystem::runFolder + "planet/" + planet.ID()) });
		fileSystem::rocketFiles[planet.ID()].addcolumns({"time"});
	}
	for (const auto& planet : *objects::physicsPlanets) {
		fileSystem::planetFiles.insert({ planet.ID(), WriteFile<ld>(fileSystem::runFolder + "planet/" + planet.ID()) });
		fileSystem::rocketFiles[planet.ID()].addcolumns({"time"});
	}
}

void loggingEnd() {
	createGeneralRunInfo();

	for (const auto& rocket : *objects::rockets) {
		loggRocket(rocket);
		for (const auto& rocketStage : rocket.stages()) {
			loggRocketStage(rocketStage, rocket.ID() + toS(rocketStage.ID()));
			for (const auto& engine : rocketStage.engines())
				loggEngine(engine, rocket.ID() + toS(rocketStage.ID()) + toS(engine.ID()));
			for (const auto& fuelTank : rocketStage.fuelTanks())
				loggFuelTank(fuelTank, rocket.ID() + toS(rocketStage.ID()) + toS(fuelTank.ID()));
		}
	}
	for (const auto& planet : *objects::physicsPlanets)
		loggPlanet(planet);
	for (const auto& planet : *objects::fixedOrbitPlanets)
		loggPlanet(planet);
}

void createGeneralRunInfo() {
	std::ofstream file((fileSystem::runFolder + "GeneralRunInfo").cstr());
	file << "randomSeed=" << toS(options::randomSeed) << std::endl;
	file << "totalTime=" << toS(objects::time);
	file.close();
}

/*
endres slik at dem ikke lagres hver gang men mår dem forandre seg for rocketStage, fueltank og engine
kansje også slik at fixedOrbitPlanet ikke treng å bli lagra siden den hlold sæ stabil på en bane
*/

void loggCurrentState() {
	for (const auto& rocket : *objects::rockets) {
		loggRocket(rocket);
		for (const auto& rocketStage : rocket.stages()) {
			loggRocketStage(rocketStage, rocket.ID()+toS(rocketStage.ID()));
			for (const auto& engine : rocketStage.engines())
				loggEngine(engine, rocket.ID() + toS(rocketStage.ID()) + toS(engine.ID()));
			for (const auto& fuelTank : rocketStage.fuelTanks())
				loggFuelTank(fuelTank, rocket.ID() + toS(rocketStage.ID()) + toS(fuelTank.ID()));
		}
	}
	for (const auto& planet : *objects::physicsPlanets)
		loggPlanet(planet);
	for(const auto& planet : *objects::fixedOrbitPlanets)
		loggPlanet(planet);
}

void loggRocket(const Rocket& rocket) {
	fileSystem::rocketFiles[rocket.ID()].addData({ objects::time,
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
	fileSystem::rocketStageFiles[globalID].addData({ objects::time,
		});
}

void loggEngine(const Engine& engine, const String& globalID) {
	fileSystem::engineFiles[globalID].addData({ objects::time,
		});
}

void loggFuelTank(const FuelTank& fuelTank, const String& globalID) {
	fileSystem::fuelTankFiles[globalID].addData({ objects::time,
		});
}

void loggPlanet(const PhysicsPlanet& planet) {
	fileSystem::planetFiles[planet.ID()].addData({objects::time,
		});
}

void loggPlanet(const FixedOrbitPlanet& planet) {
	fileSystem::planetFiles[planet.ID()].addData({ objects::time,
		});
}