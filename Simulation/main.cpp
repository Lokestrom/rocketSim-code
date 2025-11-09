#include "ui/openGL/OpenGLApp.hpp"

#include "ui/openGL/windows/startup/Launcher.hpp"

#include "Simulation/Simulation.hpp"

int main()
{
	OpenGL::App::startup();

	OpenGL::App::addWindow(std::make_unique<OpenGL::Launcher>());

	while (OpenGL::App::update());

	OpenGL::App::shutdown();

	return 0;
}