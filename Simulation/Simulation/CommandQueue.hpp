#pragma once

#include <queue>
#include <mutex>
#include <variant>

#include "Entities/PhysicsBody.hpp"

class SimulationContainer;
class Simulation;

class CommandQueue
{
public:
	enum class CommandType {
		ADD_ENTITY,
		REMOVE_ENTITY,
		REMOVE_ALL
	};

	enum class ComponetType {
		TEST_COMPONENT,
	};

	struct AddComponentData 
	{
		ComponetType type;
		PhysicsBodyBuilder component;
	};

	struct Command {
		CommandType type;
		// TODO: allow for different command data types
		std::variant<std::monostate, ID::UUID, AddComponentData> data;
	};

	CommandQueue(SimulationContainer& container);

	size_t pushCommand(Command&& command);
	void popCommands();

	ID::UUID getEntityUUID(size_t id) noexcept {
		std::lock_guard<std::mutex> lock(_mutex);
		if(!_entityIDs.contains(id))
			return ID::UUID{};
		return _entityIDs.erase(id);
	}

private:
	void processAddComponentCommand(AddComponentData& data);
	void processRemoveAllCommand();
	void processRemoveEntityCommand(ID::UUID id);

	std::queue<Command> _commands;
	std::mutex _mutex;

	std::unordered_map<size_t, ID::UUID> _entityIDs;

	Simulation& _simulation;
	SimulationContainer& _container;
};