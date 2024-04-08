#ifndef _EPIG_H
#define _EPIG_H
#include "policy.h"

class EPIG_alg : public Policy {
	private:
	// State transitionCount[numStates][ACTION_SIZE][???]; // IS THETA_HAT
	int seed = 0;
	bool DEBUG = false;
	int EPSILON = 5;
	double KL_D(StateTransitionVector* Theta, StateTransitionVector* ThetaHat);
	double GetProbability(StateTransition sas);
	double GetProbability(StateTransitionVector sas, StateCounter s);
	double GetProbability(StateTransitionVector sas, int stateIndex);
	double GetProbability(StateCounter s, int totalCount);
	double GetProbability(int visitCount, int totalCount);
	void Initialize();
	//int* MakeNewCount(int *curCountVector, int changedCount);
	double PIG(State s, Action a);
	public:
		EPIG_alg(int seed, int numStates, int numActions);
		EPIG_alg(int seed, int Epsilon, int numStates, int numActions);
		~EPIG_alg();
		Action getAction(State s);
		void Update(StateTransition sas);
		void PrintTransitionVectors(State s, Action a);
		//void Update(StateTransition *sas);
};

#endif
