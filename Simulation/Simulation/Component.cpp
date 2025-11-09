#include "Component.hpp"

#include "Simulation.hpp"

Component::Component(Simulation& simulation, const ComponentBuilder& builder)
	: _id(ID::createID(builder.name, builder.localID)), 
	_simulation(simulation),
	_transform(builder.transform)
{}


Component::Component(Component&& component) noexcept
	: _simulation(component._simulation),
	_id(std::move(component._id)),
	_transform(std::move(component._transform))
{}
