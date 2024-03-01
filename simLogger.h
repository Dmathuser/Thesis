#ifndef _SIM_LOGGER_H
#define _SIM_LOGGER_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <simulation.h>
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
	public:
		SimLogger(Simulation *simulation);
		SimLogger(Simulation *simulation, int simSeed);
		~SimLogger();
		bool startLog();
		bool stopLog();
		bool printSimLogs(std::string outFileName);
		bool isPolicyOptimal();
};

#endif
