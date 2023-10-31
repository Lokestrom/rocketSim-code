#pragma once
#include "controles.hpp"

namespace options
{
	inline sizeT edgeDetectionIterations = 0;
	inline sizeT pointApproximationOfMeshesPerM2 = 0;
	inline sizeT physicsTimestepSize = 1;
	inline sizeT cashSize = 10;
	inline unsigned randomSeed;
}

namespace flags {
	enum class gravitySimulationTypes{
		newton, 
		generalRelativity
	};
	inline gravitySimulationTypes gravitySimulation;
	enum class aerodynamicSimulationTypes{
		simple, 
		CFD
	};
	inline aerodynamicSimulationTypes aerodynamicSimulation;

	inline bool random;
};