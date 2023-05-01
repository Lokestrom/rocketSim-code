#include "Vector3.hpp"
#include "DragSim.hpp"
#include "Quaternion.hpp"
#include "controles.hpp"
#include "planet.hpp"

#include <iostream>
#include <math.h>

void update() {
	for (auto& i : *planets) {
		i.earlyUpdate();
	}
	for (auto& i : *rockets) {
		i.update();
	}
	for (auto& i : *planets) {
		i.update();
	}

	logState();
}

using namespace objects;
using namespace Database;
int main() {
	planets = new Vector<Planet>();
	rockets = new Vector<Rocket>();
	objects::time = 0;



}