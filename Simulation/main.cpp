#include "Vector3.hpp"
#include "DragSim.hpp"
#include "Quaternion.hpp"
#include "controles.hpp"
#include <iostream>
#include <math.h>

using namespace objects;
using namespace Database;
int main() {
	rocketStages = new Vector<Vector<RocketStage>>();
	planets = new Vector<Planet>();
	rockets = new Vector<Rocket>();
	objects::time = 0;


}