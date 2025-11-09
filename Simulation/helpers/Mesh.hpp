#pragma once

#include "math.hpp"
#include "controles.hpp"
#include "options.hpp"
#include "../SimulationModel.hpp"
#include "ID.hpp"


class BoundingSphre {
public:
	BoundingSphre(const Vector<Vector3>& vertices);

	void isInsideOtherBoundingSphre(const Vector3& pos, Vector<ID::UUID>& ids);
private:
	ld _radius;
};

Vector<Vector2> getFacing2DBoundry(const SimulationModel& model, const Vector3& dir);
ld getFacingSurfaceArea(const SimulationModel& model, const Vector3& dir);