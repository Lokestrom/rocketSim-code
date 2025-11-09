#pragma once

#include <queue>
#include <mutex>
#include <variant>

#include "Simulation/Component.hpp"

class Simulation;

class CommandQueue
{
public:
	enum class CommandType {
		ADD_COMPONENT,
	};

	enum class ComponetType {
		TEST_COMPONENT,
	};

	struct AddComponentData 
	{
		ComponetType type;
		std::unique_ptr<ComponentBuilder> component;
	};

	struct Command {
		CommandType type;
		// TODO: allow for different command data types
		std::variant<AddComponentData> data;
	};

	CommandQueue(Simulation& simulation);

	void pushCommand(Command&& command);
	void popCommands();

private:
	void processAddComponentCommand(const AddComponentData& data);

	std::queue<Command> _commands;
	std::mutex _mutex;
	Simulation& _simulation;
};