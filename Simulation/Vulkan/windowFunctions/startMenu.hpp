#pragma once

#include "../App.hpp"

struct StartMenu {
	static inline String name = "start menu";
	static inline WindowType type = WindowType::Startmenu;
	struct Info {
	
	};

	static void load(WindowInfo& window);
	static void open(WindowInfo& window);
	static void swapTo(WindowInfo& window);
	static void* createInfo();
};
