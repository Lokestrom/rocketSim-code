#pragma once

#include "ECS/Simulation.hpp"

constexpr bool performanceProfiling = true;
constexpr double BenchmarkDT = 1E-3;
constexpr size_t benchmarkTimeSeconds = 10;

void simulationBenchmarkRun();

void timeSimulationBenchmarkRun(Simulation&& sim);
void profileSimulationBenchmarkRun(Simulation&& sim);

Simulation simBuilder();