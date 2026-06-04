#ifndef _NEPIG_H
#define _NEPIG_H
#include "policy.h"

class NEPIG_alg : public Policy {
	private:
	// State transitionCount[numStates][ACTION_SIZE][???]; // IS THETA_HAT
	int seed = 0;
	unsigned int rand_seed = 0;
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
		NEPIG_alg(int seed, int numStates, int numActions);
		NEPIG_alg(int seed, int Epsilon, int numStates, int numActions);
		~NEPIG_alg();
		Action getAction(State s);
		void Update(StateTransition sas);
		void PrintTransitionVectors(State s, Action a);
		//void Update(StateTransition *sas);
};

#endif
