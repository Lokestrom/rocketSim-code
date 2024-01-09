#pragma once

#include "startMenu.hpp"
#include "mainMenu.hpp"
#include "alarms.hpp"
#include "freeCam.hpp"
#include "instructions.hpp"
#include "loadInConfig.hpp"
#include "loadInRun.hpp"
#include "mapView.hpp"
#include "newSimulation.hpp"
#include "options.hpp"
#include "telemetry.hpp"
#include "timeControles.hpp"
#include "openWindowFunctions.hpp"
#include "WindowTypeSpecificInfo.hpp"

#include "../callbackFunctions.hpp"

void createWindowData() {
	windowTypeData::addWindow<StartMenu>();
}