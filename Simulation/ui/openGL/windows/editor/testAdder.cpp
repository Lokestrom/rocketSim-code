#include "testAdder.hpp"

#include "ui/ImGui/imgui.h"

#include "ui/openGL/OpenGLApp.hpp"

#include "Simulation/components/Planet.hpp"

OpenGL::testAdder::testAdder()
	: ImguiWindow("test adder")
{
}

void loadExample() {
	PlanetBuilder testObject{};

	testObject.name = "test object 1";
	testObject.localID = 0;
	testObject.mass = 1E14;

	OpenGL::App::getSimulationContainer().getCommandQueue().pushCommand(CommandQueue::Command{
	.type = CommandQueue::CommandType::ADD_COMPONENT,
	.data = CommandQueue::AddComponentData{
		.type = CommandQueue::ComponetType::TEST_COMPONENT,
		.component = std::make_unique<PlanetBuilder>(testObject)
	}
	});

	testObject.name = "test object 2";
	testObject.localID = 0;
	testObject.transform.translation = Vector3(10, 0, 0);
	testObject.velosity = Vector3(0, 25.8347, 0);
	testObject.mass = 1;

	OpenGL::App::getSimulationContainer().getCommandQueue().pushCommand(CommandQueue::Command{
		.type = CommandQueue::CommandType::ADD_COMPONENT,
		.data = CommandQueue::AddComponentData{
			.type = CommandQueue::ComponetType::TEST_COMPONENT,
			.component = std::make_unique<PlanetBuilder>(testObject)
		}
	});
}

bool OpenGL::testAdder::render()
{
	bool open = true;

	static bool error;
	static std::string msg;

	ImGui::Begin(name.c_str(), &open);
	static float position[3] = { 0.0f, 0.0f, 0.0f };
	ImGui::InputFloat3("Position", position);
	static float velosity[3] = { 0.0f, 0.0f, 0.0f };
	ImGui::InputFloat3("Velosity", velosity);
	static double mass = 0;
	ImGui::InputDouble("Mass", &mass);

	if (error) {
		ImGui::TextColored({ 0.8, 0.2, 0.2, 1.0 }, msg.c_str());
	}

	if (ImGui::Button("add test object")) {
		if (mass <= 0) {
			error = true;
			msg = "Mass must be grater than 0";
		}
		else {
			error = false;
			msg = "";

			PlanetBuilder testObject;
			testObject.name = "test object";
			testObject.localID = 0;
			testObject.transform.translation = Vector3(position[0], position[1], position[2]);
			testObject.velosity = Vector3(velosity[0], velosity[1], velosity[2]);
			testObject.mass = mass;
			App::getSimulationContainer().getCommandQueue().pushCommand(CommandQueue::Command{
				.type = CommandQueue::CommandType::ADD_COMPONENT,
				.data = CommandQueue::AddComponentData{
					.type = CommandQueue::ComponetType::TEST_COMPONENT,
					.component = std::make_unique<PlanetBuilder>(testObject)
				}
			});
		}
	}

	ImGui::NewLine();
	if (ImGui::Button("Add example")) {
		loadExample();
	}

	ImGui::End();
	return open;
}