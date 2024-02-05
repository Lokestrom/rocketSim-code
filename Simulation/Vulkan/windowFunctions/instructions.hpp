#pragma once

#include "windowFunctions.hpp"

namespace windows {

	class Instructions {
	public:
		static inline String name = "Instructions";
		static inline windows::Type type = windows::Type::Instructions;
		struct Info {
			Vector<String> openFiles;
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};
}