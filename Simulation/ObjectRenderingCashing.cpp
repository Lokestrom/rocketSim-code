#include "ObjectRenderingCashing.hpp"

#include "rocket/rocket.hpp"
#include "planet.hpp"
#include <iostream>
#include "helpers/simulationObjects.hpp"

void ObjectRenderingCash::addsimulationTimeCash(const SimulationTimeCash& simulationTime)
{
	std::lock_guard<std::mutex> lock(mtx);

	size++;
	_cash.pushOn(simulationTime);
}

SimulationTimeCash ObjectRenderingCash::getsimulationTimeCash(ld time)
{
	std::lock_guard<std::mutex> lock(mtx);

	if (size == 0)
		return SimulationTimeCash();
	SimulationTimeCash ans;
	do {
		ans = std::move(_cash.pushOff());
		size--;
	} while (ans.time <= time-timeObjects::dt && !(size == 0));
	std::cout << "Sim cash time: " << ans.time << "\n";
	std::cout << "real time: " << time << "\n";
	return ans;
}

ld ObjectRenderingCash::getNextCashTime()
{
	std::lock_guard<std::mutex> lock(mtx);

	return _cash.viewFront().time;
}

sizeT ObjectRenderingCash::getSize()
{
	return size;
}

void addSimulationTransforms()
{
	SimulationTimeCash cash;
	cash.time = timeObjects::currentTime;
	for (const auto& rocket : objectLists::rockets) {
		for (const auto& rocketStage : rocket->stages()) {
			cash.objects[rocketStage->getID().getID()] = TotalTransformComponent3D(*rocketStage->getTransform());
			for (const auto& engine : rocketStage->getEngines()) {
				cash.objects[engine->getID().getID()] = TotalTransformComponent3D(*engine->getTransform());
			}
			for (const auto& fuelTank : rocketStage->getFuelTanks()) {
				cash.objects[fuelTank->getID().getID()] = TotalTransformComponent3D(*fuelTank->getTransform());
			}
		}
	}

	for (const auto& planet : objectLists::physicsPlanets) {
		cash.objects[planet->getID().getID()] = TotalTransformComponent3D(*planet->getTransform());
	}
	for (const auto& planet : objectLists::fixedOrbitPlanets) {
		cash.objects[planet->getID().getID()] = TotalTransformComponent3D(*planet->getTransform());
	}
	objectLists::objectCash.addsimulationTimeCash(cash);
}
