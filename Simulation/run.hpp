#pragma once

#include "String.hpp"

using namespace Database;

bool update();

void run();

void loadSimulationFiles(String folder, String runName);
void deloadSimulation();