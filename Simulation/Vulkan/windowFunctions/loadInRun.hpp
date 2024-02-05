#pragma once

#include "windowFunctions.hpp"

namespace windows {

	class LoadInRun {
	public:
		static inline String name = "Load in existing run";
		static inline windows::Type type = windows::Type::LoadInRun;
		struct Info {

		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};
}