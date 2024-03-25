#include <iostream>
#include <stdlib.h>
#include "policy.h"

Policy::Policy()
{
  states=actions=0;
  transitionCount = NULL;
}

Policy::Policy(int states, int actions)
{
  this->states = states;
  this->actions = actions;
  transitionCount = new StateTransitionVector*[states];
  for(int i = 0; i < states; i++)
    {
      transitionCount[i] = new StateTransitionVector[actions];
    }
  std::cout << "Policy initialized with "<<states<<" states and "<<actions<<" actions.\n";
}


Policy::~Policy()
{
  if(transitionCount != NULL)
    {
      for(int i = 0; i < states; i++)
			{
				delete[] transitionCount[i];
			}      
      delete[] transitionCount;
    }
}

Action Policy::getAction(State s)
{
	Action a = UP;
	return a;
}

void Policy::Update(StateTransition sas)
{
	int s = sas.s.StateId;
	StateCounter *tempStateCounter = nullptr;

	//Update counter for transition observed
	if (findStateCounterIndex(&transitionCount[s][sas.a].sPrime, &sas.sPrime, &tempStateCounter))
		tempStateCounter->count += 1;
	else //Add observed transition to known transitions if it wasn't there.
	{
		tempStateCounter = new StateCounter;
		State ObservedState = {sas.sPrime.StateId,sas.sPrime.Noise};
		transitionCount[s][sas.a].sPrime.push_back(*tempStateCounter);
		transitionCount[s][sas.a].sPrime.back().count += 1;
		tempStateCounter = &transitionCount[s][sas.a].sPrime.back(); 
		copy_State(&ObservedState, &tempStateCounter->s);
	}
	//increase counter for total number of observed transitions.
	transitionCount[s][sas.a].totalCount += 1;
}
