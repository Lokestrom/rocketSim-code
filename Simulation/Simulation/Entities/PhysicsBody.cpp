#include "PhysicsBody.hpp"

#include "../ECS/forceGenerators.hpp"
#include "../ECS/Simulation.hpp"

PhysicsBody::PhysicsBody(Simulation& sim, Builder builder)
{
	auto& entities = sim.getEntityContainer();

	_id = entities.addEntity(builder.name, builder.localID).getID().getUUID();
	entities.addComponent<Position>(_id, builder.position);
	entities.addComponent<Velosity>(_id, builder.velocity);
	entities.addComponent<Orientation>(_id, builder.orientation);
	entities.addComponent<Mass>(_id, builder.mass);
	entities.addComponent<Scale>(_id, builder.scale);

	glm::mat<3, 3, ld> inertiaTensor = cuboidInertiaTensor(builder.mass, builder.scale);
	entities.addComponent<InertiaTensor>(_id, inertiaTensor);
	entities.addComponent<InverseInertiaTensor>(_id, glm::inverse(inertiaTensor));

	glm::vec3 angMom = inertiaTensor * builder.angularVelocity.asGLM();
	entities.addComponent<AngularMomentum>(_id, Vector3(angMom.x, angMom.y, angMom.z));
	entities.addComponent<Model3D>(_id, GPU::Model::LoadFromFile(builder.modelPath.cstr()));

	auto& integrators = sim.getIntegratorContainer();
	integrators.add(_id);
	integrators.addForceGenerator(_id, ForceGenerator(gravity));
}
