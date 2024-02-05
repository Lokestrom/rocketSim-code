#pragma once

#include "windowFunctions.hpp"

namespace windows {
	class LoadInConfig {
	public:
		static inline String name = "Load in existing config";
		static inline Type type = Type::LoadInConfig;
		struct Info {
			String folder;
			String runName;
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);

	private:
		static void setFolder(WindowInfo& window, String folder);
		static void setRunName(WindowInfo& window, String runName);
		static void startLoad(WindowInfo& window);
	};
}