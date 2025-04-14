#include "loadInConfig.hpp"

#include "../windowFunctions.hpp"
#include "../../App.hpp"
#include "../../run.hpp"
#include "../helpers/TextInputPopup.hpp"
#include <filesystem>

using namespace windows;

void LoadInConfig::load(WindowInfo& window)
{
    Vector2 buttonSize = { .5,.1 };
    Vector3 buttonColor = { 0,0,1 };

    auto grid = createGrid({ 1,3 }, { .8, .4 }, { 0,0 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    std::unordered_map<String, std::shared_ptr<TextInputField>> textFields;

    textFields["folder"] = TextInputField::createTextInputField(window, grid[0][0], buttonSize);
    textFields["folder"]->changeColor(window, buttonColor);

    StaticText::createText(window, { grid[0][0].x, grid[0][0].y - .15}, textColor, textScale, "Folder path:");

    textFields["runname"] = TextInputField::createTextInputField(window, grid[0][1], buttonSize);
    textFields["runname"]->changeColor(window, buttonColor);

    StaticText::createText(window, { grid[0][1].x, grid[0][1].y-.15 }, textColor, textScale, "Run name:");

    auto button = Button::createButton(window, grid[0][2], buttonSize);
    button->changeColor({0,0,0}, window);

    auto forum = Forum::createForum(window, button, textFields);
    forum->setSubmitFunction(startLoad);

    StaticText::createText(window, { grid[0][2].x, grid[0][2].y }, textColor, textScale, "Load");
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

void windows::LoadInConfig::startLoad(WindowInfo& window, Forum::Info* values)
{
    if (!std::filesystem::exists(values->data["folder"].cstr())) {
        Warning("The folder: \"" + values->data["folder"] + "\" does not exsist\nEnter a new folder", Warning::Type::BadInput, closeWindow);
        return;
    }

    loadSimulationFiles(values->data["folder"], values->data["runname"]);
    getSwapFunction(Type::Menu)(window);

    delete values;
}
