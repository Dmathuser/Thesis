#ifndef _NRAND_WALK_H
#define _NRAND_WALK_H
#include "policy.h"

class NRandWalk : public Policy {
	private:
	// int transitionCount[4][4][ACTION_SIZE][2];
	int seed = 0;
	unsigned int rand_seed = 0;
	void Initialize();
	public:
  NRandWalk(int seed,int numStates,int numActions);
		~NRandWalk();
		Action getAction(State s);
		void Update(StateTransition sas);
		void Update(StateTransition *sas);
};

#endif
