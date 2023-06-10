#pragma once

#include "fileSystem.hpp"

namespace fileSystem {
	class Instructions {
	public:
		Instructions();
		Instructions(String fileName, Rocket* rocket);

		Instructions operator=(const Instructions& ins) {
			_file.open(toSTD(ins._rocket->ID() + ".txt"));
			if (!_file.is_open())
				throw error("File \"" + ins._rocket->ID() + ".txt\" couldn't be opened.", high);
			_rocket = ins._rocket;
			getInstruction();
		}

		void run();
	
	private: 
		Vector<String> returnInstructions(const String& line);
		void getInstruction();
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