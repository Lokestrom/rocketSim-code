#include "loadInConfig.hpp"

#include "windowFunctions.hpp"
#include "../App.hpp"
#include "../../run.hpp"

using namespace windows;

void LoadInConfig::load(WindowInfo& window)
{
    Vector2 buttonSize = { 1.1,.3 };
    Vector3 buttonColor = { 0,0,1 };

    Vector<Vector2> grid = createGrid({ 1,3 }, { .8, .4 }, { 0,0 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    auto textInputField = TextInputField::createTextInputField(window, grid[0], buttonSize);
    textInputField->changeColor(window, buttonColor);
    textInputField->setSubmitFunction(setFolder);

    StaticText::createText(window, { grid[0].x, grid[0].y }, textColor, textScale, "Folder path:");

    textInputField = TextInputField::createTextInputField(window, grid[1], buttonSize);
    textInputField->changeColor(window, buttonColor);
    textInputField->setSubmitFunction(setRunName);

    StaticText::createText(window, { grid[0].x, grid[0].y }, textColor, textScale, "Run name:");

    auto button = Button::createButton(window, grid[2], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(startLoad);

    StaticText::createText(window, { grid[0].x, grid[0].y }, textColor, textScale, "Folder path:");
}
void LoadInConfig::open(WindowInfo& window)
{
	createWindow(type);
}
void LoadInConfig::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* LoadInConfig::createInfo()
{
	return new Info();
}
void LoadInConfig::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}

void windows::LoadInConfig::setFolder(WindowInfo& window, String folder)
{
    Info* info = (Info*)window.typeSpecificInfo;
    
    info->folder = folder;
}

void windows::LoadInConfig::setRunName(WindowInfo& window, String runName)
{
    Info* info = (Info*)window.typeSpecificInfo;

    info->runName = runName;
}

void windows::LoadInConfig::startLoad(WindowInfo& window)
{
    Info* info = (Info*)window.typeSpecificInfo;

    loadSimulationFiles(info->folder, info->runName);
    getSwapFunction(Type::Menu)(window);
}
