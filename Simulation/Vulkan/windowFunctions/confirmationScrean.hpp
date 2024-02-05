#pragma once

#include "windowFunctions.hpp"

namespace windows {
	class ConfirmationScrean
	{
	public:
		static inline String name = "Confirmaton";
		static inline Type type = Type::Confirmation;
		struct Info {
			void (*yes)(WindowInfo&);
			void (*no)(WindowInfo&);
			
			String text;
		};

		static void load(WindowInfo& window);
		static void open(WindowInfo& window);
		static void swapTo(WindowInfo& window);
		static void* createInfo();
		static void deleteInfo(void* info);

		static void open(WindowInfo& window, Info* info);
		static void swapTo(WindowInfo& window, Info* info);
	};
}

