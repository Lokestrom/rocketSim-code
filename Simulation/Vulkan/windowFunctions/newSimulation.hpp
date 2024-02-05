#pragma once

#include "windowFunctions.hpp"

namespace windows {

	class NewSimulation {
	public:
		static inline String name = "Create new simulation";
		static inline Type type = Type::NewSimulation;
		struct Info {

		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};
}