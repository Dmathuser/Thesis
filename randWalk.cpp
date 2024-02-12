#include "randWalk.h"
#include <stdlib.h>

RandWalk::RandWalk(int seed,int numStates,int numActions):Policy(numStates,numActions)
{
	seed = seed;
	srand(seed); //Might be slow...
}

RandWalk::~RandWalk()
{
}

Action RandWalk::getAction(State s)
{
	Action a;
	a = Action(rand()%ACTION_SIZE);

	return a;
}
