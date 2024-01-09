#pragma once
#include "../../rocket/Rocket.hpp"
#include "../GameObject.hpp"

class GameObject3D;

enum class WindowType {
	Startmenu,
	NewSimulation,
	LoadInConfig,
	LoadInRun,
	Menu,
	Telemetry,
	FreeCam,
	Options,
	Alarms,
	MapView,
	Instructions,
	Time
};

class windowTypeData {
public:
	struct Data
	{
		const char* name;
		void (*loadFunction)(WindowInfo& window);

		void (*openFunctionButton)(WindowInfo& window);
		void (*swapFunctionButton)(WindowInfo& window);

		void* (*createInfoFunction)();
	};

	static Data& getWindowData(WindowType type) {
#ifdef _DEBUG
		if (!dataMap.count(type))
			throw error("there is no window data defined for the WindowType: " + (int)type);
#endif // _DEBUG
		return dataMap.at(type);
	}

	template<class T>
	static void addWindow() {
		Data data;
#ifdef _DEBUG
		if (dataMap.count(T::type))
			throw error("there can't be multiple datas for window type: " + toS((int)T::type), exitCodes::codeFault);
		try {
#endif // _DEBUG
			data.name = T::name.cstr();
			data.loadFunction = T::load;
			data.openFunctionButton = T::open;
			data.swapFunctionButton = T::swapTo;
			data.createInfoFunction = T::createInfo;
#ifdef _DEBUG
		}
		catch () {
			throw error("mising definisinon for a function for window: " + toS((int)T::type), exitCodes::codeFault);
		}
#endif // _DEBUG

		dataMap.insert({ T::type, data });
	}

private:
	static inline std::unordered_map<WindowType, Data> dataMap;
};

struct LoadInConfigWindowInfo {

};

struct LoadInRunWindowInfo {

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

void* getWindowSpecificInfo(WindowType type) {
	return windowTypeData::getWindowData(type).createInfoFunction();
}