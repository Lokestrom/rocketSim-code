#pragma once

#include "windowFunctions.hpp"

namespace windows {

	class Alarms {
	public:
		static inline String name = "Alarms";
		static inline Type type = Type::Alarms;
		struct Info {

		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};
}