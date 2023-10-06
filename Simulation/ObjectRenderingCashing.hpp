#pragma once

#include "Vector.hpp"
#include "String.hpp"
#include "Queue.hpp"

#include "helpers/TransformComponent3D.hpp"
#include "helpers/controles.hpp"

#include <unordered_map>

using namespace Database;

struct SimulationTimeCash {
	ld time;
	std::unordered_map<ID::GlobaleID_T, TotalTransformComponent3D> objects;
};

class ObjectRenderingCash
{
public:
	void addsimulationTimeCash(const SimulationTimeCash& simulationTime);
	SimulationTimeCash getsimulationTimeCash(ld time);
	sizeT getSize();
private:
	sizeT size = 0;
	Queue<SimulationTimeCash> _cash;
};

void addSimulationTransforms();

namespace objectLists {
	inline ObjectRenderingCash objectCash;
}


