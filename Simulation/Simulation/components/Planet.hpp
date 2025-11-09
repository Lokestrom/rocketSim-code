#pragma once
#include "../behaviors/GravityBehaviorObject.hpp"
#include "../behaviors/model3DBehaviorObject.hpp"
#include "../behaviors/RungeKutta4BehaviorObject.hpp"

struct PlanetBuilder 
	: public ComponentBuilder
{
	double mass;
	Vector3 velosity;
};

class Planet
	: public Component,
	public IGravityComponent,
	public IRungeKutta4
{
public:
	using Builder = PlanetBuilder;

	Planet(Simulation& simulation, const Builder& builder);
	GravityObject gravity;
	Model3DObject model3D;
	RungeKutta4Object rungeKutta4;

	//~Planet() override {}

	double getMass() const override{
		return mass;
	}
	const Vector3& getPosition() const override {
		return _transform.translation;
	}
	void applyForce(const Vector3& force) override {
	}

	void changePosition(const Vector3& delta) override  {
		_transform.translation += delta;
	}

	GPU::Model3D& getModel3D() {
		return model3D.getModel3D();
	}
private:
	double mass;
};