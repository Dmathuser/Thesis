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
  transitionCount = new int**[states];
  for(int i = 0; i < states; i++)
    {
      transitionCount[i] = new int*[actions];
      for(int j=0;j<actions;j++)
	transitionCount[i][j] = new int[2]{0,0};
    }
  std::cout << "Policy initialized with "<<states<<" states and "<<actions<<" actions.\n";
}


Policy::~Policy()
{
  if(transitionCount != NULL)
    {
      for(int i = 0; i < states; i++)
	{
	  for(int j=0;j<actions;j++)
	    delete[] transitionCount[i][j];
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
	// int curCount = transitionCount[s][sas.a][0];
	//int TotCount = stateProbability[sas.s][sas.a][1];
	transitionCount[s][sas.a][0] += 1; //increase counter for sas observed
	transitionCount[s][sas.a][1] += 1; //increase counter for total sa.
}
