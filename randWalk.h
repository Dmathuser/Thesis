#ifndef _RAND_WALK_H
#define _RAND_WALK_H
#include "policy.h"

class RandWalk : public Policy {
	private:
	// int transitionCount[4][4][ACTION_SIZE][2];
	int seed = 0;
	void Initialize();
	public:
  RandWalk(int seed,int numStates,int numActions);
		~RandWalk();
		Action getAction(State s);
		//void Update(StateTransition sas);
		//void Update(StateTransition *sas);
};

#endif
