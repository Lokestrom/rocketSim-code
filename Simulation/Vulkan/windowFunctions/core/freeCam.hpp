#pragma once

#include "../windowFunctions.hpp"
#include "../../helpers/math/Vector3.hpp"
#include <memory>

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

		static void clear3dModels(WindowInfo& window);
		static void load3dModels(WindowInfo& window);
		static void setCameraPosition(WindowInfo& window, String position);

		static Vector3 getRocketVel();
		static Vector3 getRocketAcc();
		static Vector3 getRocketPos();
	};
}