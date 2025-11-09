#pragma once
#include "../ImguiWindow.hpp"

namespace OpenGL
{

class testAdder :
    public ImguiWindow
{
public:
    testAdder();
	bool render() override;
};

}


