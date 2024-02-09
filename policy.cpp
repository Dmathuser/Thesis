#include "policy.h"

Policy::Policy()
{
}

Policy::~Policy()
{
}

Action Policy::getAction(State s)
{
	Action a = UP;
	return a;
}

void Policy::Update(StateTransition sas)
{
	int s = sas.s.StateId;
	int curCount = transitionCount[s][sas.a][0];
	//int TotCount = stateProbability[sas.s][sas.a][1];
	transitionCount[s][sas.a][0] += 1; //increase counter for sas observed
	transitionCount[s][sas.a][1] += 1; //increase counter for total sa.
}
