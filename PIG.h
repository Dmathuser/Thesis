#ifndef _PIG_H
#define _PIG_H
#include "policy.h"

class PIG_alg : public Policy {
	private:
	// int transitionCount[4][4][ACTION_SIZE][2]; // IS THETA_HAT
	int seed = 0;
	bool DEBUG = false;
	double KL_D(int* Theta, int* ThetaHat);
	double GetProbability(StateTransition sas);
	double GetProbability(int s, Action a, int sPrime);
	int* MakeNewCount(int *curCountVector, int changedCount);
	double PIG(State s, Action a);
	public:
		PIG_alg(int seed, int numStates, int numActions);
		~PIG_alg();
		Action getAction(State s);
		void Update(StateTransition sas);
		//void Update(StateTransition *sas);
};

#endif
