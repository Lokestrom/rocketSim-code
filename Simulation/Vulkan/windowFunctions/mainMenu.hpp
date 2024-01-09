#pragma once

#include "../App.hpp"

class MainMenu {
	static inline String name = "main menu";
	static inline WindowType type = WindowType::Startmenu;
	struct Info {

	};

	static void load(WindowInfo& window);
	static void open(WindowInfo& window);
	static void swapTo(WindowInfo& window);
	static void* createInfo();
};

