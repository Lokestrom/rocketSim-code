#include "options.hpp"

#include "../windowFunctions.hpp"

#include "../../UI.hpp"
#include "../../App.hpp"
#include "../../Camera.hpp"

using namespace windows;

void Options::load(WindowInfo& window)
{
	//ad a way of seting the camera position
    Vector2 buttonSize = { .5,.15 };
    Vector3 buttonColor = { 0,0,1 };

    auto grid = createGrid({ 2,4 }, { .8, .4 }, { -0.4,-.5 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };
}



void Options::open(WindowInfo& window)
{
	createWindow(type);
}
void Options::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* Options::createInfo()
{
	return new Info();
}
void Options::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}