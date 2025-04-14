#include "RocketInstructions.hpp"

#include "fileSystem.hpp"
#include "../helpers/simulationObjects.hpp"

using namespace Database;
namespace fileSystem {
	Instructions::Instructions() {
		_rocket = nullptr;
	}
	Instructions::Instructions(std::shared_ptr<Rocket> rocket) {
		_file.open(toSTD(objects::simulationFolder + "rocket/instructions/" + rocket->getID().getName() + ".txt"));
		if (!_file.is_open())
			Error("File \"" + rocket->getID().getName() + ".txt" + "\" couldn't be opened.", Error::Type::fileFault);
		_rocket = rocket;
		getInstruction();
	}
	Instructions::Instructions(String fileName, std::shared_ptr<Rocket> rocket) {
		_file.open(toSTD(objects::simulationFolder + "rocket/instructions/" + fileName));
		if (!_file.is_open())
			Error("File \"" + fileName + "\" couldn't be opened.", Error::Type::fileFault);
		_rocket = rocket;
		getInstruction();
	}

	void Instructions::run(bool& exitSimulation) {
		while (timeObjects::currentTime >= _nextInstructionTime) {
			for (auto& i : _nextInstruction) {
				if (i.contains("=")) {
					Vector<String> variable = returnVariableAndValue(i);
					setVariable(variable[0], variable[1]);
					continue;
				}
				if (i.contains("(")) {
					runInstruction(std::move(i), exitSimulation);
					if (exitSimulation)
						return;
					continue;
				}
				Error("The instruction \"" + i + "\" is not valid." +
					"\nThe instruction must be formed like this:\n" +
					"\"timestamp:instruction(args)\" or \"timestamp:setting=value", 
					Error::Type::badUserBehavior);
			}
			getInstruction();
		}
	}

	void Instructions::getInstruction() {
		String line;
		if (!getline(_file, line)) {
			_nextInstructionTime = LDBL_MAX;
			_nextInstruction = { "no more instructions in file" };
			return;
		}
		line.remove(' ');
		line.lower();
		_nextInstructionTime = STold(line.split(':')[0]);
		_nextInstruction = returnInstructions(line.split(':')[1]);
	}

	Vector<String> Instructions::returnInstructions(const String& line) {
		String instructoin;
		Vector<String> ans(5);
		for (auto i = 0; i < line.length(); i++) {
			instructoin.pushBack(line[i]);
			if (line[i] == '(') {
				for (i++; i < line.length() && line[i] != ')'; i++) {
					instructoin.pushBack(line[i]);
				}
				instructoin.pushBack(')');
				i++;
				ans.pushBack(instructoin);
				instructoin.clear();
			}
			if (line[i] == '=') {
				for (i++; i < line.length() && line[i] != ','; i++) {
					instructoin.pushBack(line[i]);
				}
				ans.pushBack(instructoin);
				instructoin.clear();
			}
		}
		return ans;
	}

	void Instructions::setVariable(const String& variable, const String& value) {
		if (variable == "deltatime")
			timeObjects::dt = STold(value);
		else if (variable == "edgedetectioniterations")
			options::edgeDetectionIterations = SToull(value);
		else if (variable == "pointapproximationofmeshesperm2")
			options::pointApproximationOfMeshesPerM2 = SToull(value);
		else if (variable == "dtinstancesperlogging")
			options::physicsTimestepSize = SToull(value);

		else
			 Error("The variable \"" + variable + "\" is not a valid variable.", 
				Error::Type::badUserBehavior);
	}

	void Instructions::runInstruction(String instruction, bool& exitSimulation) {
		Vector<String> args = returnArgs(instruction);
		instruction = instruction.split('(')[0];
		if (instruction == "burn") {
			switch (args.size())
			{
			case 0:
				_rocket->burn();
				break;
			case 1:
				_rocket->burn(STold(args[0]));
				break;
			case 2:
				_rocket->burn(STold(args[0]), returnVectorID(args[1]));
				break;
			default:
				Error("The Instruction \"burn\" at timestamp\"" + toS(_nextInstructionTime) + "\" has to many arguments or a speling error.",
					Error::Type::badUserBehavior);
				break;
			}
		}
		else if (instruction == "shutdown") {
			switch (args.size())
			{
			case 0:
				_rocket->shutdown();
				break;
			case 1:
				_rocket->shutdown(returnVectorID(args[0]));
				break;
			default:
				Error("The Instruction \"shutdown\" at timestamp\"" + toS(_nextInstructionTime) + "\" has to many arguments or a speling error.",
					Error::Type::badUserBehavior);
				break;
			}
		}
		else if (instruction == "exit") {
			exitSimulation = true;
			return;
		}

		else if (instruction == "no more instructions in file")
			_nextInstructionTime = LDBL_MAX;
		else
			Error("Instruction \"" + instruction + "\" at timestamp\"" + _nextInstructionTime + "\" is not a valid instruction.",
				Error::Type::badUserBehavior);
		exitSimulation = false;
	}

	void assignRocketInstructions(std::shared_ptr<Rocket> rocket) {
		objectLists::instructions.pushBack(std::make_shared<Instructions>(rocket->getID().getName() + ".txt", rocket));
	}
} //fileSystem