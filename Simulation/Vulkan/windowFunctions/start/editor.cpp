#include "editor.hpp"

#include "../windowFunctions.hpp"
#include "../../UI.hpp"

#include "../../run.hpp"

#include "../../App.hpp"

#include "../../FileSystem/fileSystem.hpp"

#include "../../helpers/physics.hpp"

#include <filesystem>

using namespace windows;

void Editor::load(WindowInfo& window)
{
	Vector2 buttonSize = { .8,.15 };
	Vector3 buttonColor = { 0,0,1 };

	auto grid = createGrid({ 1,3 }, { .8, .4 }, { 0,-.5 });

	float textScale = 0.00005;
	glm::vec4 textColor = { 1,0,0,1 };

	auto button = Button::createButton(window, grid[0][0], buttonSize);
	button->changeColor(buttonColor, window);
	button->setClickFunction(reset);

	StaticText::createText(window, { grid[0][0].x, grid[0][0].y }, textColor, textScale, "Refrech");

	std::unordered_map<String, std::shared_ptr<TextInputField>> textFields;
	textFields["type"] = TextInputField::createTextInputField(window, grid[0][1], buttonSize);

	StaticText::createText(window, { grid[0][2].x, grid[0][2].y }, textColor, textScale, "Create object");
	
	button = Button::createButton(window, grid[0][2], buttonSize);

	auto forum = Forum::createForum(window, button, textFields);
	forum->setSubmitFunction(createObjectFile);

	Background::createBackground(window, { 0,1,1 });

	createWindow(Type::Menu);
}
void Editor::open(WindowInfo& window)
{
	createWindow(type);
}
void Editor::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* Editor::createInfo()
{
	return new Info();
}
void Editor::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}

void Editor::reset(WindowInfo& window)
{
	::reset();
}

void Editor::createObjectFile(WindowInfo& window, Forum::Info* values)
{
	static int n = 1;

	String type = values->data["type"];
	if (type == "rocket" || type == "r") {
		std::ofstream file(toSTD(fileSystem::objects::simulationFolder + "\\rocket\\rocket\\" + toS(n) + ".txt"));
		file << "setup{\n"
			<< "pos = {" << sqrt(((n*4)*(n*4))/2) * pow(-1, n) << ", " << sqrt(((n * 4) * (n * 4)) / 2) * pow(-1, n) << ", 1 }\n"
			<< "vel = {" << sqrt((sqrt((G*1E13)/(n*4))* sqrt((G * 1E13) / (n * 4)))/2) * -pow(-1, n) <<", "<< sqrt((sqrt((G * 1E13) / (n * 4)) * sqrt((G * 1E13) / (n * 4))) / 2) *pow(-1, n) <<", 0 }\n"
			<< "acc = {0,0,0}\n"
			<< "orientation = Quat{" << (n - 1) * 45 << "," << (n - 1) * 15 << "," << (n - 1) * 30 << "}\n"
			<< "}\n"
			<< "stage{\n"
			<< "0 = stage1({0,0,0})\n"
			<< "}\n";
		file.close();
		std::ofstream file2(toSTD(fileSystem::objects::simulationFolder + "\\rocket\\instructions\\" + toS(n) + ".txt"));
		file2 << "9999:exit()";
		file2.close();
	}
	else if (type == "rocketStage" || type == "rs") {
		//createRocket(values);
	}
	else if (type == "planet" || type == "p") {
		//createPlanet(values);
	}
	else if (type == "engine" || type == "e") {
		//createEngine(values);
	}
	else if (type == "fuelTank" || type == "f") {
		//createFuelTank(values);
	}
	else {
		Warning("The type: \"" + type + "\" is not a valid type", Warning::Type::BadInput, closeWindow);
	}
	n++;
}