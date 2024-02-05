#include "controles.hpp"

#include "../Vulkan/windowFunctions/windowFunctions.hpp"
#include "../Vulkan/windowFunctions/error.hpp"
#include "simulationObjects.hpp"

Error::Error(const String& what, exitCodes code, recoveryType recoveryType, String function, String file, int line)
{
	_what = what;
	_code = code;
	_recoveryType = recoveryType;

	_function = function;
	_file = file;
	_line = line;

	_duringRecoveryAttempt = attemptingRecovery;

	list.push_back(*this);

	if (attemptingRecovery)
		return;

	if (recoveryAttempt(recoveryType))
		return;

#ifdef _DEBUG
	 throw "what: " + _what
		 + "\nfunction: " + _function
		 + "\nfile: " + _file
		 + "\nline: " + _line
		 + "\n";
#else
	windows::createWindow(windows::Type::ErrorList);
	auto info = (windows::ErrorPopup::Info*)windows::createInfo(windows::Type::ErrorPopup);
	info->error = this;
	createWindow(windows::Type::ErrorPopup, info);
#endif // _DEBUG
}

bool Error::recoveryAttempt(recoveryType recoveryType)
{
	attemptingRecovery = true;
	bool succses;

	if (!recoveryFunctions.count(recoveryType))
		succses = false;
	else
		succses = recoveryFunctions.at(recoveryType)();

	attemptingRecovery = false;
	return succses;
}

bool Error::rerunSimulationRecovery()
{
	//setSimulationToTime(timeObjects::currentTime--);
	return false;

}

bool Error::deloadSimulation()
{
	return false;
}
