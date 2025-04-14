#pragma once

#include "String.hpp"

using namespace Database;

bool update();

void run();

void reLoadSimulationFiles();
void loadSimulationFiles(String folder, String runName);
void deloadSimulation();

void startSimulationRun();
void endSimulationRun();

void startSimulation();
void stopSimulation();

void reset();