#pragma once
#include "controles.hpp"

#include <chrono>

class SimulationObject;

namespace objectLists
{
	inline Vector<std::shared_ptr<SimulationObject>> simulationObjects;
	inline Vector<std::shared_ptr<FixedOrbitPlanet>> fixedOrbitPlanets;
	inline Vector<std::shared_ptr<PhysicsPlanet>> physicsPlanets;
	inline Vector<std::shared_ptr<Rocket>> rockets;
	inline Vector<std::shared_ptr<fileSystem::Instructions>> instructions;
}

namespace timeObjects {
	inline ld currentTime = 0;
	inline ld dt = 0;
	inline sizeT dtInstancesSinceLastLogging = 0;

	inline ld realStartTimeEpoch;
	inline ld realRunTime;
	inline ld pauseStartTimeEpoch;

	void updateTime() noexcept;
	inline ld getTimeSinceEpoch() noexcept {
		return std::chrono::duration<ld>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
}