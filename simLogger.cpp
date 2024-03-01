#include "simLogger.h"

SimLogger::SimLogger(Simulation *simulation)
{
	if (simulation != nullptr)
		sim = simulation;
	else
		std::cout << "Error: Bad Simulation Input" << std::endl;
}

SimLogger::SimLogger(Simulation *simulation, int simSeed)
{
	logs.seed = simSeed;
	if (simulation != nullptr)
		sim = simulation;
	else
		std::cout << "Error: Bad Simulation Input" << std::endl;
}

SimLogger::~SimLogger()
{
}
bool SimLogger::startLog()
{
	startTime = std::chrono::high_resolution_clock::now();
	return false;
}
bool SimLogger::stopLog()
{
	stopTime = std::chrono::high_resolution_clock::now();
	using FloatMilli = std::chrono::duration<double, std::chrono::milliseconds::period>;
	//logs.learningRateCount = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count();
	logs.learningRateCount = FloatMilli(stopTime - startTime).count();
	//logs.learningRateCount.count() = duration.count();
	return false;
}
bool SimLogger::printSimLogs(std::string outFileName)
{
	std::cout << "Time taken by function: "
         << logs.learningRateCount << " milliseconds" << std::endl;
	return false;
}
bool SimLogger::isPolicyOptimal()
{
	return false;
}

