#pragma once

#include "../windowFunctions.hpp"
#include "../../UI.hpp"

namespace windows {
	class LoadInConfig {
	public:
		static inline String name = "Load in existing config";
		static inline Type type = Type::LoadInConfig;
		struct Info {
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);

	private:
		static void startLoad(WindowInfo& window, Forum::Info* values);
	};
}