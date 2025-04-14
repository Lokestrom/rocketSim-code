#pragma once

#include "../windowFunctions.hpp"

namespace windows {
	class ErrorList
	{
	public:
		static inline String name = "Error list";
		static inline Type type = Type::ErrorList;
		struct Info {

		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);
	};

	class ErrorPopup
	{
	public:
		static inline String name = "Error";
		static inline Type type = Type::ErrorPopup;
		struct Info {
			Error* error;
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);

		static void open(Info* info);
		static void swapTo(WindowInfo& window, Info* info);
	};

	class WarningPopup
	{
	public:
		static inline String name = "Warning";
		static inline Type type = Type::WarningPopup;
		struct Info {
			Warning* warning;
			void (*continueFunction)(WindowInfo&) = nullptr;
			void (*changeFunction)(WindowInfo&) = nullptr;
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);

		static void open(Info* info);
		static void swapTo(WindowInfo& window, Info* info);
	};
}

