#pragma once
#include "../../helpers/controles.hpp"

struct WindowInfo;
class GameObject3D;

namespace windows {
	void createWindowData();

	enum class Type {
		StartMenu,
		NewSimulation,
		Editor,
		LoadInConfig,
		LoadInRun,

		Menu,
		Telemetry,
		FreeCam,
		Options,
		Alarms,
		MapView,
		Instructions,
		Time,

		Confirmation,
		TextInputPopup,

		ErrorList,
		ErrorPopup,
		WarningPopup
	};

	struct Data
	{
		const char* name;
		void (*loadFunction)(WindowInfo& window);

		void (*openFunctionButton)(WindowInfo& window);
		void (*swapFunctionButton)(WindowInfo& window);

		void* (*createInfoFunction)();
		void (*deleteInfoFunction)(void* info);
		

		template<class T>
		static constexpr void addWindow() {
			Data data;
#ifdef _DEBUG
			if (dataMap.count(T::type))
				Error("there can't be multiple datas for window type: " + toS((int)T::type), Error::Type::codeFault);
			try {
#endif // _DEBUG
				data.name = T::name.cstr();
				data.loadFunction = T::load;
				data.openFunctionButton = T::open;
				data.swapFunctionButton = T::swapTo;
				data.createInfoFunction = T::createInfo;
				data.deleteInfoFunction = T::deleteInfo;
#ifdef _DEBUG
			}
			catch (...) {
				Error("mising definisinon for a function for window: " + toS((int)T::type), Error::Type::codeFault);
			}
#endif // _DEBUG

			dataMap.insert({ T::type, data });
		}

		static Data& getWindowData(Type type) {
#ifdef _DEBUG
			if (!dataMap.count(type))
				Error("there is no window data defined for the WindowType: " + (int)type, Error::Type::codeFault);
#endif // _DEBUG
			return dataMap.at(type);
		}

	private:
		static inline std::unordered_map<Type, Data> dataMap;
	};


	String getName(Type type);
	void(*getLoadFunction(Type type))(WindowInfo& window);
	void(*getOpenFunction(Type type))(WindowInfo& window);
	void(*getSwapFunction(Type type))(WindowInfo& window);
	void* createInfo(Type type);
	void(*getDeleteInfoFunction(Type type))(void* info);

	void closeWindow(WindowInfo& window);

	WindowInfo createWindowInfo(windows::Type type, void* info = nullptr);
	void swapWindowType(WindowInfo& window, windows::Type newType, void*  info = nullptr);
	void createWindow(windows::Type type, void* info = nullptr);
}