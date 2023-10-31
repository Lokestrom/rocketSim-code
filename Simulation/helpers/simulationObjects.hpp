#pragma once
#include "controles.hpp"

namespace objectLists
{
	inline Vector<std::shared_ptr<FixedOrbitPlanet>> fixedOrbitPlanets;
	inline Vector<std::shared_ptr<PhysicsPlanet>> physicsPlanets;
	inline Vector<std::shared_ptr<Rocket>> rockets;
	inline Vector<std::shared_ptr<fileSystem::Instructions>> instructions;
}

namespace timeObjects {
	inline ld currentTime = 0;
	inline ld dt = 0;
	inline sizeT dtInstancesSinceLastLogging = 0;

	inline ld realStartTime;
	inline ld realCurrentTime;

	void updateTime() noexcept;
}