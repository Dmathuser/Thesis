#ifndef _NPIG_H
#define _NPIG_H
#include "policy.h"

class NPIG_alg : public Policy {
	private:
	// State transitionCount[numStates][ACTION_SIZE][???]; // IS THETA_HAT
	int seed = 0;
	unsigned int rand_seed = 0;
	bool DEBUG = false;
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
		NPIG_alg(int seed, int numStates, int numActions);
		~NPIG_alg();
		Action getAction(State s);
		void Update(StateTransition sas);
		void PrintTransitionVectors(State s, Action a);
		//void Update(StateTransition *sas);
};

#endif
