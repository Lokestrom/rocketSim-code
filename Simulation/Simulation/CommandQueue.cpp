#include "CommandQueue.hpp"

#include "Simulation/Simulation.hpp"

#include "components/Planet.hpp"

#include <memory>

CommandQueue::CommandQueue(Simulation& simulation)
	: _simulation(simulation)
{
}

void CommandQueue::pushCommand(Command&& command) {
	std::lock_guard<std::mutex> lock(_mutex);
	_commands.push(std::move(command));
}

void CommandQueue::popCommands() {
	std::lock_guard<std::mutex> lock(_mutex);

	while (!_commands.empty()) {
		auto& command = _commands.front();
		switch (command.type) {
		case CommandType::ADD_COMPONENT:
			processAddComponentCommand(std::get<AddComponentData>(command.data));
			break;
		default:
			break;
		}
		_commands.pop();
	}
}

void CommandQueue::processAddComponentCommand(const AddComponentData& data)
{
	switch (data.type) {
	case ComponetType::TEST_COMPONENT:
		auto& builder = *static_cast<PlanetBuilder*>(data.component.get());
		auto component = std::make_unique<Planet>(
			_simulation,
			builder
		);
		_simulation.addComponent(std::move(component));
	}
}
