#include "ObjectRenderingCashing.hpp"

#include "rocket/rocket.hpp"
#include "planet.hpp"
#include <iostream>

void ObjectRenderingCash::addsimulationTimeCash(const SimulationTimeCash& simulationTime)
{
	size++;
	_cash.pushOn(simulationTime);
}

SimulationTimeCash ObjectRenderingCash::getsimulationTimeCash(ld time)
{
	if (size == 0)
		return SimulationTimeCash();
	SimulationTimeCash ans = std::move(_cash.pushOff());
	size--;
	while (ans.time < time && !(size == 0)) {
		ans = std::move(_cash.pushOff());
		size--;
	}
	std::cout << "Sim cash time: " << ans.time << "\n";
	std::cout << "real time: " << time << "\n";
	return ans;
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
			cash.objects[rocketStage->getID().getID()] = *rocketStage->getTransform();
			for (const auto& engine : rocketStage->getEngines()) {
				cash.objects[engine->getID().getID()] = *engine->getTransform();
			}
			for (const auto& fuelTank : rocketStage->getFuelTanks()) {
				cash.objects[fuelTank->getID().getID()] = *fuelTank->getTransform();
			}
		}
	}

	for (const auto& planet : objectLists::physicsPlanets) {
		cash.objects[planet->getID().getID()] = *planet->getTransform();
	}
	for (const auto& planet : objectLists::fixedOrbitPlanets) {
		cash.objects[planet->getID().getID()] = *planet->getTransform();
	}
	objectLists::objectCash.addsimulationTimeCash(cash);
}
