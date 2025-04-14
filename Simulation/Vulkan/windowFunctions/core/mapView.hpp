#pragma once

#include "../windowFunctions.hpp"

namespace windows {

	class MapView {
	public:
		static inline String name = "Map view";
		static inline windows::Type type = windows::Type::MapView;
		struct Info {
			std::shared_ptr<GameObject3D> followingOBJ;
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};
}