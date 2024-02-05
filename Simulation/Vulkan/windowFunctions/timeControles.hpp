#pragma once

#include "windowFunctions.hpp"

namespace windows {

	class TimeControles {
	public:
		static inline String name = "time controles";
		static inline windows::Type type = windows::Type::Time;
		struct Info {

		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};

}


