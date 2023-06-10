#pragma once

namespace flags {
	enum gravitySimulationStyles{newton, generalRelativity};
	inline int gravitySimulation;
	enum aerodynamicSimulationStyles{simple, CFD};
	inline int aerodynamicSimulation;
	inline bool random;
};