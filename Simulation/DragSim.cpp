#include "DragSim.hpp"
#include "controles.hpp"
#include "Mesh.hpp"
#include "Vector3.hpp"
#include <iostream>


//test
Vector3 Drag::evaluateDrag(Sphere stage) {
	sizeT particleNum = 1E6;
	sizeT vel = 30;
	Vector<Sphere> particles(particleNum);
	ld force = 0;

	for (sizeT i = 0; i < particleNum; i++) {
		particles.pushBack(Sphere(1E-4, { static_cast <ld> (rand()) / static_cast <ld> (RAND_MAX) * 30, static_cast <ld> (rand()) / static_cast <ld> (RAND_MAX) * 30, static_cast <ld> (rand()) / static_cast <ld> (RAND_MAX) * 30 }));
	}

	for (auto& i : particles) {
		if (i.pos.length() + i.radius < stage.radius) {
			force += 4E-3 * 30;
		}
	}

	return { 0,0,force };
}