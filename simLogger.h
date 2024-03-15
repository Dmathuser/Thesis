#ifndef _SIM_LOGGER_H
#define _SIM_LOGGER_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <math.h>
#include "simulation.h"
#include "policy.h"
#include "enums.h"
#include "structs.h"

//template <typename T> 
//const std::chrono::duration<T> PERIOD_LENGTH;
//const std::chrono::milliseconds PERIOD_LENGTH;

class SimLogger {
	private:
		SimLogs logs;
		std::chrono::time_point<std::chrono::system_clock> startTime;
		std::chrono::time_point<std::chrono::system_clock> stopTime;
		Simulation *sim = nullptr;
		Policy *policy = nullptr;
		std::ofstream fout;
		double KL_D(int* Theta, int* ThetaHat);
		double getPolicyAccuracy();
	public:
		SimLogger(Simulation *simulation, Policy *policy);
		SimLogger(Simulation *simulation, int simSeed);
		SimLogger(Simulation *simulation, Policy *policy_, int simSeed);
		~SimLogger();
		bool startLog();
		bool stopLog();
		bool log();
		bool printSimLogs(std::string outFileName);
		bool isPolicyOptimal();
};

#endif
