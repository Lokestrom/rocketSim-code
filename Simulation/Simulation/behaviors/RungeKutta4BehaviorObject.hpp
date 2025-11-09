#pragma once

#include "rungeKutta4.hpp"
#include "../BehaviorObject.hpp"

class RungeKutta4Object 
	: public BehaviorObject
{
public:
	template<isComponent C>
	RungeKutta4Object(C& component, Simulation& sim)
		: BehaviorObject(component, std::move(std::make_unique<RungeKutta4>(static_cast<IRungeKutta4&>(component), sim)), sim)
	{ }
};