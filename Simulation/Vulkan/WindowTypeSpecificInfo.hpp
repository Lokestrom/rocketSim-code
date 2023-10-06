#pragma once
#include "../rocket/Rocket.hpp"
#include "GameObject.hpp"

class GameObject3D;

enum class WindowType {
	General,
	Menu,
	Telemetry,
	FreeCam,
	Options,
	Alarms,
	MapView,
	Instructions,
	Time
};

struct MainWindowInfo {

};

struct OptionsWindowInfo {

};

enum class TelemetryType {
	Rocket,
	PhysicsPlanet,
	FixedOrbitPlanet
};
struct TelemetryWindowInfo {
	std::shared_ptr<PhysicsPlanet> physicsPlanet;
	std::shared_ptr<FixedOrbitPlanet> fixedOrbitPlanet;
	std::shared_ptr<Rocket> rocket;
	TelemetryType type;

	std::shared_ptr<TransformComponent3D> relativeObj;

};

struct InstructionsWindowInfo {
	Vector<String> openFiles;
};

struct FreeCamWindowInfo {
	std::shared_ptr<GameObject3D> followingOBJ;
};

struct MapViewWindowInfo {
	std::shared_ptr<GameObject3D> followingOBJ;
};

struct AlarmWindowInfo {

};

struct TimeWindowInfo {
};