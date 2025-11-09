#pragma once

#include "../ImguiWindow.hpp"

namespace OpenGL
{
	class TimeWindow
		: public ImguiWindow
	{
		public:
		TimeWindow() : ImguiWindow("time window") {}
		bool render() override;
	};

}