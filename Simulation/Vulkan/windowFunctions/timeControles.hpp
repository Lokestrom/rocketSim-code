#pragma once

#include "../App.hpp"

class TimeControles {
	static inline String name = "time controles";
	static inline WindowType type = WindowType::Time;
	struct Info {

	};

	static void load(WindowInfo& window);
	static void open(WindowInfo& window);
	static void swapTo(WindowInfo& window);
	static void* createInfo();
};


