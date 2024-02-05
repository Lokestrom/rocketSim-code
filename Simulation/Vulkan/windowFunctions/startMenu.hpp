#pragma once

#include "windowFunctions.hpp"

namespace windows {

	struct StartMenu {
	public:
		static inline String name = "start menu";
		static inline Type type = Type::StartMenu;
		struct Info {
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};
}