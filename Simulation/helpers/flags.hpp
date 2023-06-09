#pragma once

namespace flags {
	enum gravitySimulationStyles{newton, generalRelativity};
	int gravitySimulation;
	enum aerodynamicSimulationStyles{simple, CFD};
	int aerodynamicSimulation;
	bool random;
};