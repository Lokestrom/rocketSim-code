#include "newSimulation.hpp"

#include "editor.hpp"

#include "../windowFunctions.hpp"
#include "../../UI.hpp"

#include "../../run.hpp"

#include "../../App.hpp"

#include <filesystem>
#include "../../../FileSystem/fileSystem.hpp"

using namespace windows;

void NewSimulation::load(WindowInfo& window)
{
#ifdef TESTING
	std::filesystem::remove_all("testing\\newSim");
    fileSystem::createFileTemplate("testing\\newSim");

    loadSimulationFiles("testing\\newSim", "test");
    getSwapFunction(Type::Editor)(window);
    return;
#endif // TESTING
    Vector2 buttonSize = { .5,.1 };
    Vector3 buttonColor = { 0,0,1 };

    auto grid = createGrid({ 1,4 }, { .8, .4 }, { 0,-.5 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    std::unordered_map<String, std::shared_ptr<TextInputField>> textFields;

    textFields["folder"] = TextInputField::createTextInputField(window, grid[0][0], buttonSize);
    textFields["folder"]->changeColor(window, buttonColor);

    StaticText::createText(window, { grid[0][0].x, grid[0][0].y - .15 }, textColor, textScale, "Folder path:");

    textFields["configName"] = TextInputField::createTextInputField(window, grid[0][1], buttonSize);
    textFields["configName"]->changeColor(window, buttonColor);

    StaticText::createText(window, { grid[0][1].x, grid[0][1].y - .15 }, textColor, textScale, "Config name:");

    textFields["runname"] = TextInputField::createTextInputField(window, grid[0][2], buttonSize);
    textFields["runname"]->changeColor(window, buttonColor);

    StaticText::createText(window, { grid[0][2].x, grid[0][2].y - .15 }, textColor, textScale, "Run name:");

    auto button = Button::createButton(window, grid[0][3], buttonSize);
    button->changeColor({ 0,0,0 }, window);
    StaticText::createText(window, { grid[0][3].x, grid[0][3].y }, textColor, textScale, "Setup");

    auto forum = Forum::createForum(window, button, textFields);
    forum->setSubmitFunction(startSetup);
}
void NewSimulation::open(WindowInfo& window)
{
	createWindow(type);
}
void NewSimulation::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* NewSimulation::createInfo()
{
	return new Info();
}
void NewSimulation::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}

void NewSimulation::startSetup(WindowInfo& window, Forum::Info* values)
{
    if (!std::filesystem::exists(values->data["folder"].cstr())) {
        Warning("The folder: \"" + values->data["folder"] + "\" does not exsist\nEnter a new folder", Warning::Type::BadInput, closeWindow);
        return;
    }
    if (std::filesystem::exists((values->data["folder"] + "\\" + values->data["configName"]).cstr())) {
        Warning("The folder: \"" + values->data["folder"] + "\\" + values->data["configName"] + "\" does already exsist \nEnter a new folder name", Warning::Type::BadInput, closeWindow);
        return;
    }

    fileSystem::createFileTemplate(values->data["folder"] + "\\" + values->data["configName"]);

    loadSimulationFiles(values->data["folder"] + "\\" + values->data["configName"], values->data["runname"]);
    getSwapFunction(Type::Editor)(window);

    delete values;
}
