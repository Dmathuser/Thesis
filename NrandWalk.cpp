#include "NrandWalk.h"
#include <stdlib.h>

NRandWalk::NRandWalk(int seed,int numStates,int numActions):Policy(numStates,numActions)
{
	seed = seed;
	rand_seed = seed; //Might be slow...
	Initialize();
}

NRandWalk::~NRandWalk()
{
}

void NRandWalk::Initialize()
{
	transitionCount = NULL;
	//Loop through transitionCount and update s,a values 
	// for (int s=0; s < states; s++)
	// 	for (int a=0; a < actions; a++)
	// 	{
	// 		transitionCount[s][a].s.StateId = s;
	// 		transitionCount[s][a].s.Noise = 0;
	// 		transitionCount[s][a].a = Action(a);
	// 		transitionCount[s][a].totalCount = 1;
	// 	}
}

Action NRandWalk::getAction(State s)
{
	Action a;
	a = Action(rand_r(&rand_seed)%ACTION_SIZE);

	return a;
}

void NRandWalk::Update(StateTransition sas)
{
	int s = sas.s.StateId;
	StateCounter *tempStateCounter = nullptr;
	StateTransitionVector *transCountNew = new StateTransitionVector;

	if (transitionMemory.find(hash_sa(sas.s, sas.a)) == transitionMemory.end()) 
	{
		transCountNew->s.StateId = sas.s.StateId;
		transCountNew->s.Noise = sas.s.Noise;
		transCountNew->a = sas.a;
		transCountNew->totalCount = 1;
		copy_StateTransitionVector(transCountNew, &transitionMemory[hash_sa(sas.s, sas.a)]);
	// 		transitionCount[s][a].s.StateId = s;
	// 		transitionCount[s][a].s.Noise = 0;
	// 		transitionCount[s][a].a = Action(a);
	// 		transitionCount[s][a].totalCount = 1;
	}
	//Update counter for transition observed
	else if (findStateCounterIndex(&transitionMemory[hash_sa(sas.s,sas.a)].sPrime, &sas.sPrime, &tempStateCounter))
		tempStateCounter->count += 1;
	else //Add observed transition to known transitions if it wasn't there.
	{
		tempStateCounter = new StateCounter;
		transitionMemory[hash_sa(sas.s,sas.a)].sPrime.push_back(*tempStateCounter);
		transitionMemory[hash_sa(sas.s,sas.a)].sPrime.back().count += 1;
		//Free up pointer
		delete tempStateCounter;

		tempStateCounter = &transitionMemory[hash_sa(sas.s,sas.a)].sPrime.back(); 
		State ObservedState = {sas.sPrime.StateId, sas.sPrime.Noise};
		copy_State(&ObservedState, &tempStateCounter->s);
	}

	//increase counter for total number of observed transitions.
	transitionMemory[hash_sa(sas.s,sas.a)].totalCount += 1;
}
