#pragma once

#include "../windowFunctions.hpp"

namespace windows {
	class TextInputPopup
	{
	public:
		static inline String name = "Text input";
		static inline Type type = Type::TextInputPopup;
		struct Info {
			void(*returnFunc)(std::string) = nullptr;
			String what;
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);

		static void open(WindowInfo& window, Info* info);
		static void swapTo(WindowInfo& window, Info* info);

	private:
		static void runReturnFunc(WindowInfo& window, String str);
	};
}

