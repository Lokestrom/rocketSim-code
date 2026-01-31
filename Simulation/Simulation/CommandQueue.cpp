#include "CommandQueue.hpp"
#include "CommandQueue.hpp"
#include "CommandQueue.hpp"

#include "ECS/Simulation.hpp"
#include "Container.hpp"

#include <memory>

CommandQueue::CommandQueue(SimulationContainer& container)
	: _simulation(container.getSimulation()),
	_container(container)
{
}

size_t CommandQueue::pushCommand(Command&& command) {
	static size_t comandID = 0;
	std::lock_guard<std::mutex> lock(_mutex);
	_commands.push(std::move(command));
	return comandID++;
}

void CommandQueue::popCommands() {
	std::lock_guard<std::mutex> lock(_mutex);

	while (!_commands.empty()) {
		auto& command = _commands.front();
		switch (command.type) {
		case CommandType::ADD_ENTITY:
			processAddComponentCommand(std::get<AddComponentData>(command.data));
			break;

		case CommandType::REMOVE_ENTITY:
			processRemoveEntityCommand(std::get<ID::UUID>(command.data));
			break;
		
		case CommandType::REMOVE_ALL:
			processRemoveAllCommand();
			break;
		
		default:
			break;
		}
		_commands.pop();
	}
}

void CommandQueue::processAddComponentCommand(AddComponentData& data)
{
	_container.getRenderingCache().invalidate();
	switch (data.type) {
	case ComponetType::TEST_COMPONENT:
		PhysicsBody(_simulation, data.component);
	}
}

void CommandQueue::processRemoveAllCommand()
{
	_container.getRenderingCache().invalidate();
	_simulation.getEntityContainer().clear();
	_simulation.getIntegratorContainer().clear();
}

void CommandQueue::processRemoveEntityCommand(ID::UUID id)
{
	_simulation.getEntityContainer().removeEntity(id);
	_simulation.getIntegratorContainer().remove(id);
}
