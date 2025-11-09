#pragma once

#include "Database/Vector.hpp"
#include "Database/String.hpp"
#include "Database/Queue.hpp"

#include "helpers/TransformComponent3D.hpp"
#include "helpers/controles.hpp"
#include "helpers/ID.hpp"

#include <unordered_map>
#include <mutex>

struct SimulationTimeCash {
	ld time;
	std::unordered_map<ID::UUID, TotalTransformComponent3D> objects;
};

class ObjectRenderingCash
{
public:
	void addsimulationTimeCash(const SimulationTimeCash& simulationTime);
	SimulationTimeCash getsimulationTimeCash(ld time);

	ld getNextCashTime();
	sizeT getSize();

	void clear();
private:
	std::mutex mtx;
	sizeT size = 0;
	Queue<SimulationTimeCash> _cash;
};

void addSimulationTransforms();

namespace objectLists {
	inline ObjectRenderingCash objectCash;
}


