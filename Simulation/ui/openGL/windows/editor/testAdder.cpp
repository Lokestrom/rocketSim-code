#include "testAdder.hpp"

#include "ui/ImGui/imgui.h"

#include "ui/openGL/OpenGLApp.hpp"
#include "Simulation/Entities/PhysicsBody.hpp"

OpenGL::testAdder::testAdder()
	: ImguiWindow("test adder")
{
}

void loadExample() {
	PhysicsBodyBuilder testObject{};

	testObject.name = "test object 1";
	testObject.localID = 0;
	testObject.mass = 1E14;
	testObject.modelPath = DEFAULT_CONFIG "model/cube.obj";
	testObject.angularVelocity = Vector3(2, 0, 0.1);
	testObject.scale = Vector3(1.0l, 4.0l, 0.1l);

	OpenGL::App::getSimulationContainer().getCommandQueue().pushCommand(CommandQueue::Command{
	.type = CommandQueue::CommandType::ADD_ENTITY,
	.data = CommandQueue::AddComponentData{
		.type = CommandQueue::ComponetType::TEST_COMPONENT,
		.component = testObject
	}
	});

	testObject.name = "test object 2";
	testObject.localID = 0;
	testObject.position = Vector3(10, 0, 0);
	testObject.velocity = Vector3(0, 25.8347, 0);
	testObject.mass = 1;
	testObject.angularVelocity = Vector3(0, 0, 0);
	testObject.scale = Vector3(0.1l, 0.1l, 0.1l);
	testObject.modelPath = DEFAULT_CONFIG "model/sphere.obj";

	OpenGL::App::getSimulationContainer().getCommandQueue().pushCommand(CommandQueue::Command{
		.type = CommandQueue::CommandType::ADD_ENTITY,
		.data = CommandQueue::AddComponentData{
			.type = CommandQueue::ComponetType::TEST_COMPONENT,
			.component = testObject
		}
	});

	OpenGL::App::removeLastCacheFrame();
}

Database::String getModelPath(int index) {
	switch (index) {
	case 0:
		return DEFAULT_CONFIG "model/cube.obj";
	case 1:
		return DEFAULT_CONFIG "model/sphere.obj";
	default:
		assert(false && "Invalid model index");
	}
}

bool OpenGL::testAdder::render()
{
	bool open = true;

	//TODO no need for these to be static, make them members of the class
	static bool error;
	static std::string msg;

	ImGui::Begin(name.c_str(), &open);
	static float position[3] = { 0.0f, 0.0f, 0.0f };
	ImGui::InputFloat3("Position", position);
	static float velocity[3] = { 0.0f, 0.0f, 0.0f };
	ImGui::InputFloat3("Velocity", velocity);
	static double mass = 0;
	ImGui::InputDouble("Mass", &mass);
	static float orientation[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	if (ImGui::InputFloat4("Orientation", orientation)) {
		Quaternion quat{
			static_cast<long double>(orientation[0]),
			static_cast<long double>(orientation[1]),
			static_cast<long double>(orientation[2]),
			static_cast<long double>(orientation[3])
		};
		quat = quat.normalized();
		orientation[0] = static_cast<float>(quat.w);
		orientation[1] = static_cast<float>(quat.x);
		orientation[2] = static_cast<float>(quat.y);
		orientation[3] = static_cast<float>(quat.z);
	}
	static float angularVelocity[3] = { 0.0f, 0.0f, 0.0f };
	ImGui::InputFloat3("Angular velocity", angularVelocity);

	static float scale[3] = { 1.0f, 1.0f, 1.0f };
	ImGui::InputFloat3("Scale", scale);

	static int modelIndex = 0;
	ImGui::Combo("Model", &modelIndex, "cube\0sphere\0");

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

			PhysicsBodyBuilder testObject{};
			testObject.name = "test object";
			testObject.localID = 0;
			testObject.position = Vector3(position[0], position[1], position[2]);
			testObject.velocity = Vector3(velocity[0], velocity[1], velocity[2]);
			testObject.mass = mass;
			testObject.orientation = Quaternion(
				static_cast<long double>(orientation[0]),
				static_cast<long double>(orientation[1]),
				static_cast<long double>(orientation[2]),
				static_cast<long double>(orientation[3])
			);
			testObject.orientation = testObject.orientation.normalized();
			testObject.angularVelocity = Vector3(angularVelocity[0], angularVelocity[1], angularVelocity[2]);
			testObject.modelPath = getModelPath(modelIndex);
			testObject.scale = Vector3(static_cast<long double>(scale[0]), static_cast<long double>(scale[1]), static_cast<long double>(scale[2]));
			App::getSimulationContainer().getCommandQueue().pushCommand(CommandQueue::Command{
				.type = CommandQueue::CommandType::ADD_ENTITY,
				.data = CommandQueue::AddComponentData{
					.type = CommandQueue::ComponetType::TEST_COMPONENT,
					.component = testObject
				}
			});
			OpenGL::App::removeLastCacheFrame();

		}
	}

	ImGui::NewLine();
	if (ImGui::Button("Add example")) {
		loadExample();
	}

	ImGui::NewLine();
	if (ImGui::Button("Clear")) {
		App::getSimulationContainer().getCommandQueue().pushCommand(CommandQueue::Command{
			.type = CommandQueue::CommandType::REMOVE_ALL,
			.data = std::monostate{}
			});
		OpenGL::App::removeLastCacheFrame();
	}

	ImGui::End();
	return open;
}