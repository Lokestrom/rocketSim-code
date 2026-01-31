#pragma once

#include "../windowFunctions.hpp"

#include "../../UI.hpp"	


namespace windows {

	class Editor {
	public:
		static inline String name = "Editor";
		static inline Type type = Type::Editor;
		struct Info {

		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
		static void reset(WindowInfo& window);
		static void createObjectFile(WindowInfo& window, Forum::Info* values);
	};
}