#pragma once

#include "windowFunctions.hpp"

namespace windows {
	class FreeCam {
	public:
		static inline String name = "Free cam";
		static inline Type type = Type::FreeCam;
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