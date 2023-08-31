#pragma once
#include "../rocket/Rocket.hpp"

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

struct generalWindowInfo {

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
	PhysicsPlanet& physicsPlanet;
	FixedOrbitPlanet& fixedOrbitPlanet;
	Rocket& rocket;
	TelemetryType type;

	Vector3& relativeObj;

};

struct InstructionsWindowInfo {
	Vector<String> openFiles;
};

struct FreeCamWindowInfo {
	GameObject3D& followingOBJ;
};

struct MapViewWindowInfo {
	GameObject3D& followingOBJ;
};

struct AlarmWindowInfo {

};

struct TimeWindowInfo {
};