#include "Instructions.hpp"

#include "fileSystem.hpp"
using namespace Database;
namespace fileSystem {
	Instructions::Instructions() {
		_rocket = nullptr;
	}
	Instructions::Instructions(String fileName, Rocket* rocket) {
		_file.open(toSTD(fileName));
		if (!_file.is_open())
			throw error("File \"" + fileName + "\" couldn't be opened.", high);
		_rocket = rocket;
		getInstruction();
	}
	void Instructions::run() {
		if (timeObjects::currentTime > _nextInstructionTime)
			return;
		
		try {
			for (auto& i : _nextInstruction) {
				if (i.contains("=")) {
					Vector<String> variable = returnVariableAndValue(i);
					setVariable(variable[0], variable[1]);
					continue;
				}
				if (i.contains("(")) {
					runInstruction(std::move(i));
					continue;
				}
				throw error("The instruction \"" + i + "\" is not valid." +
					"\nThe instruction must be formed like this:\n" +
					"\"timestamp:instruction(args)\" or \"timestamp:setting=value", invalidUserBehavior);
			}
		}
		catch (const error& e) {
			writeError(e, badUserBehavior);
		}
		getInstruction();
	}

	void Instructions::getInstruction() {
		String line;
		getline(_file, line);
		line.remove(' ');
		line.lower();
		ld _nextInstructionTime = STold(line.split(':')[0]);
		_nextInstruction = returnInstructions(line.split(':')[1]);
	}

	Vector<String> Instructions::returnInstructions(const String& line) {
		String instructoin;
		Vector<String> ans(5);
		for (auto i = 0; i < line.length(); i++) {
			instructoin.pushBack(i);
			if (i == '(') {
				for (; i < line.length() && line[i] != ')'; i++) {
					instructoin.pushBack(i);
				}
				i += 2;
				ans.pushBack(std::move(instructoin));
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
			options::dtInstancesPerLogging = SToull(value);

		else
			throw error("The variable \"" + variable + "\" is not a valid variable.", invalidUserBehavior);
	}

	void Instructions::runInstruction(String instruction) {
		Vector<String> args = returnArgs(instruction);
		instruction = instruction.split('(')[0];
		if (instruction == "burn") {
			switch (args.size())
			{
			case 0:
				_rocket->burn();
			case 1:
				_rocket->burn(STold(args[0]));
			case 2:
				_rocket->burn(STold(args[0]), returnVectori(args[1]));
			default:
				throw error("The Instruction \"burn\" at timestamp\"" + toS(_nextInstructionTime) + "\" has to many arguments or a speling error.", low);
			}
		}
		else if (instruction == "shutdown") {
			switch (args.size())
			{
			case 0:
				_rocket->shutdown();
			case 1:
				_rocket->shutdown(returnVectori(args[0]));
			default:
				throw error("The Instruction \"shutdown\" at timestamp\"" + toS(_nextInstructionTime) + "\" has to many arguments or a speling error.", low);
			}
		}
		else if (instruction == "exit")
			std::exit(success);

		else
			throw error("Instruction \"" + instruction + "\" at timestamp\"" + _nextInstructionTime + "\" is not a valid instruction.", low);
	}

	void assignRocketInstructions(Rocket* rocket) {
		try {
			objectLists::instructions->pushBack(Instructions(rocket->ID() + ".txt", rocket));
		}
		catch (error& e) {
			writeError(error("When assigning instructions to rocket \"" + rocket->ID() + "\" an error has acured:\n\t" + e.what), badUserBehavior);
		}
	}
} //fileSystem