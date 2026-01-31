#pragma once

#include "../windowFunctions.hpp"

namespace windows {

	class MainMenu {
	public:
		static inline String name = "main menu";
		static inline Type type = Type::Menu;
		struct Info {

		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);

	private:
		static void quitConfirm(WindowInfo& window);
		static void quit(WindowInfo& window);
	};
}

