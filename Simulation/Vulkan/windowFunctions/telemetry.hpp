#pragma once

#include "windowFunctions.hpp"

#include "../../rocket/Rocket.hpp"

namespace windows {

	class Telemetry {
	public:
		static inline String name = "Telemetry";
		static inline Type type = Type::Telemetry;
		enum class Type {
			Rocket,
			PhysicsPlanet,
			FixedOrbitPlanet
		};
		struct Info {
			std::shared_ptr<PhysicsPlanet> physicsPlanet;
			std::shared_ptr<FixedOrbitPlanet> fixedOrbitPlanet;
			std::shared_ptr<Rocket> rocket;
			Type type;

			std::shared_ptr<TransformComponent3D> relativeObj;
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);

	private:
		static void changeRelativeObject(WindowInfo& window);
		static void loadPosTelemetryView(WindowInfo& window);
		static void changeObjectInViewTelemetry(WindowInfo& window);
	};

}