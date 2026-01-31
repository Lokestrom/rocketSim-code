#include "settings.hpp"

#include "ui/openGL/OpenGLApp.hpp"
#include "ui/openGL/windows/startup/Launcher.hpp"

#include "Simulation/simBenchmark.hpp"

int main()
{
	if (!simulationBenchmark) {
		OpenGL::App::startup();
		OpenGL::App::addWindow(std::make_unique<OpenGL::Launcher>());
		while (OpenGL::App::update());
		OpenGL::App::shutdown();
	}
	else
		simulationBenchmarkRun();
	return 0;
}