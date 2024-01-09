#include "timeControles.hpp"

#include "windowFunctions.hpp"

void TimeControles::load(WindowInfo& window)
{
}

void TimeControles::open(WindowInfo& window)
{
    createWindow(type);
}

void TimeControles::swapTo(WindowInfo& window)
{
    swapWindowType(window, type);
}

void* TimeControles::createInfo()
{
    return nullptr;
}
