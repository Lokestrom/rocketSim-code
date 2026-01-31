#pragma	once

#include "helpers/math/Vector3.hpp"
#include "gpuInterface/Model.hpp"
#include "glm/mat3x3.hpp"

struct Position {
	using value_type = Vector3;
};
struct Velosity {
	using value_type = Vector3;
};
struct Mass {
	using value_type = long double;
};
struct Model3D {
	using value_type = GPU::Model3D;
};
struct Orientation {
	using value_type = Quaternion;
};
struct AngularMomentum {
	using value_type = Vector3;
};
struct InertiaTensor {
	using value_type = glm::mat<3,3,ld>;
};
struct InverseInertiaTensor {
	using value_type = glm::mat<3,3,ld>;
};
struct CenterOfMassOffset {
	using value_type = Vector3;
};
struct Scale {
	using value_type = Vector3;
};