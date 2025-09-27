#pragma once

#include "../ImguiWindow.hpp"

namespace OpenGL
{
    class Launcher 
        : public ImguiWindow
    {
    public:
        Launcher();
        bool render() override;
    };

};


