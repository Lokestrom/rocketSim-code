#pragma once

#include "fileSystem.hpp"

namespace fileSystem {
	class Instructions {
	public:
		Instructions();
		Instructions(std::shared_ptr<Rocket> rocket);
		Instructions(String fileName, std::shared_ptr<Rocket> rocket);

		void run(bool& exitSimulation);
	
	private: 
		Vector<String> returnInstructions(const String& line);
		void getInstruction();
		void setVariable(const String& variable, const String& value);
		void runInstruction(String instruction, bool& exitSimulation);
	
	private:
		std::ifstream _file;
		ld _nextInstructionTime;
		Vector<String> _nextInstruction;
		std::shared_ptr<Rocket> _rocket;
	};

	void assignRocketInstructions(std::shared_ptr<Rocket> rocket);

} // fileSystem