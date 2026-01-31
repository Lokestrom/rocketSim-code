#include "TextInputPopup.hpp"

#include "../../App.hpp"
#include "../../Text.hpp"
#include "../../UI.hpp"

#include "../../Camera.hpp"


void windows::TextInputPopup::load(WindowInfo& window)
{
    Info* info = (Info*)window.typeSpecificInfo;

    Vector2 buttonSize = { 1.1,.3 };
    Vector3 buttonColor = { 0,0,1 };

    auto grid = createGrid({ 1,2 }, { .8, .4 }, { -0.4,-.5 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    StaticText::createText(window, { grid[0][0].x, grid[0][0].y }, textColor, textScale, toSTD(info->what));

    auto input = TextInputField::createTextInputField(window, grid[0][1], buttonSize);
    input->setSubmitFunction(runReturnFunc);

    StaticText::createText(window, { grid[0][1].x, grid[0][1].y }, textColor, textScale, "Close");

    Background::createBackground(window, { 1,1,1 });

    window.camera->setting = CameraSettings::normal2d;
}

void windows::TextInputPopup::open(WindowInfo& window)
{
    assert(true && "can't open to TextInputPopup window. Action not allowed since it must have a pre configured Info");
}

void windows::TextInputPopup::swapTo(WindowInfo& window)
{
    assert(true && "can't swap to TextInputPopup window. Action not allowed since it mus have a pre configured Info");
}

void* windows::TextInputPopup::createInfo()
{
    return new Info();
}

void windows::TextInputPopup::deleteInfo(void* info)
{
    Info* infoObj = static_cast<Info*>(info);
    delete infoObj;
}

void windows::TextInputPopup::open(WindowInfo& window, Info* info)
{
    createWindow(type, info);
}

void windows::TextInputPopup::swapTo(WindowInfo& window, Info* info)
{
    swapWindowType(window, type, info);
}
void windows::TextInputPopup::runReturnFunc(WindowInfo& window, String str)
{
    Info* info = (Info*)window.typeSpecificInfo;

    info->returnFunc(toSTD(str));
    closeWindow(window);
}