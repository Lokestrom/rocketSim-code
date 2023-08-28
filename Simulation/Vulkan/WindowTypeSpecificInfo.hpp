#pragma once
#include "../rocket/Rocket.hpp"

enum class WindowType {
	Menu,
	Telemetry,
	FreeCam,
	Options,
	Alarms,
	MapView,
	Instructions
};

struct MainWindowInfo {

};

struct OptionsWindowInfo {

};

struct TelemetryWindowInfo{
	Rocket& rocket;
};

struct InstructionsWindowInfo {

};

struct FreeCamWindowInfo {

};

struct MapViewWindowInfo {

};

struct AlarmWindowInfo {

};

struct TimeWindowInfo {

};