#pragma once

#include "../windowFunctions.hpp"
#include "../../Text.hpp"

namespace windows {

	class TimeControles {
	public:
		static inline String name = "time controles";
		static inline windows::Type type = windows::Type::Time;
		struct Info {
			StaticText::id_t pauseTextID;
		};

		static void pause(WindowInfo& window);

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};

}


