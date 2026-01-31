#pragma once
#include "../ImguiWindow.hpp"

namespace OpenGL
{

    class EntityViewer :
        public ImguiWindow
    {
    public:
        EntityViewer();
        bool render() override;
    };

}


