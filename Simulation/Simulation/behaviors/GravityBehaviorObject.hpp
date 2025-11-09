#pragma once

#include "../BehaviorObject.hpp"
#include "Gravity.hpp"

class GravityObject
	: public BehaviorObject
{
public:
	template<canGravity C>
	GravityObject(C& component, Simulation& sim, GravityManager& behaviorManager)
		: BehaviorObject(component, std::move(std::make_unique<Gravity<C>>(component, behaviorManager)), sim)
	{
	}
};