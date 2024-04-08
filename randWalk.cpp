#include "randWalk.h"
#include <stdlib.h>

RandWalk::RandWalk(int seed,int numStates,int numActions):Policy(numStates,numActions)
{
	seed = seed;
	srand(seed); //Might be slow...
	//Initialize();
}

RandWalk::~RandWalk()
{
}

void RandWalk::Initialize()
{
	//Loop through transitionCount and update s,a values 
	for (int s=0; s < states; s++)
		for (int a=0; a < actions; a++)
		{
			transitionCount[s][a].s.StateId = s;
			transitionCount[s][a].s.Noise = 0;
			transitionCount[s][a].a = Action(a);
			transitionCount[s][a].totalCount = 1;
		}
}

Action RandWalk::getAction(State s)
{
	Action a;
	a = Action(rand()%ACTION_SIZE);

	return a;
}
