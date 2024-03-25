#ifndef _STRUCTS_H
#define _STRUCTS_H
#include "enums.h"
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
	std::vector<double> modelAccuracy; //KL-Divergence of true model and agent model
};

struct Index
{
	int x = -1; //Width Index
	int y = -1; //Height Index
};

struct State
{
	int StateId = -1;
	int Noise = 0;
};

struct StateTransition
{
	State s;
	Action a = UP;
	State sPrime;
};

struct StateCounter
{
	State s;
	int count = 0; //Number of Transitions to this state.
};

struct StateTransitionVector
{
	State s;
	Action a = UP;
	std::vector<StateCounter> sPrime;
	int totalCount = 0; //Total transitions using this vector.
};

const bool isStateEqual(State *s1, State *s2);
const bool isNoisyStateEqual(State *s1, State *s2);
const bool isNoisyStateEqual(State *s1, int s2_Id);
const int getNoisyStateCount(StateTransitionVector *sas, int stateId);
bool findStateCounterIndex(std::vector<StateCounter> *sVec,
	State *s, StateCounter **foundStateCounter);
bool copy_State(State *src, State *dest);
bool copy_StateCounter(StateCounter *src, StateCounter *dest);
bool copy_StateTransitionVector(StateTransitionVector *src, StateTransitionVector *dest);

#endif
