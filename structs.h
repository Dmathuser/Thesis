#ifndef _STRUCTS_H
#define _STRUCTS_H
#include <chrono>
#include <vector>

struct SimLogs //Used in SimLogger
{
	int seed = 0;
	//Time until model has an optimal solution
	//std::chrono::duration<std::chrono::system_clock> learningRateCount; 
	double learningRateCount; 
	int learningRateSteps = 0; //Time until model has an optimal solution
	//One input per time step.
	std::vector<int> distractionRate; //Number of Noisy-TV visits
	std::vector<int> modelAccuracy; //KL-Divergence of true model and agent model
};

struct Index
{
	int x = -1; //Width Index
	int y = -1; //Height Index
};
#endif
