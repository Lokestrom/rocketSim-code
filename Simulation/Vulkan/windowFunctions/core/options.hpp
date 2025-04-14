#pragma once

#include "../windowFunctions.hpp"

namespace windows {

	class Options {
	public:
		static inline String name = "Options";
		static inline windows::Type type = windows::Type::Options;
		struct Info {

		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};
}