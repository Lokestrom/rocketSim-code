#pragma once

#include "fileSystem.hpp"

namespace fileSystem {
	class Instructions {
	public:
		Instructions(String fileName, Rocket* rocket);
		void run();
	
	private: 
		Vector<String> returnInstructions(const String& line);
		Vector<String> getInstruction();
		void setVariable(const String& variable, const String& value);
		void runInstruction(String instruction);
	
	private:
		std::ifstream _file;
		ld _nextInstructionTime;
		Vector<String> _nextInstruction;
		Rocket* _rocket;
	};

	void assignRocketInstructions(Rocket* rocket);

} // fileSystem