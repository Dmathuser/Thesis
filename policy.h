#ifndef _POLICY_H
#define _POLICY_H
#include "enums.h"

struct State
{
	int StateId = -1;
	int Noise = 0;
};

struct StateTransition
{
	State s;
	Action a;
	State sPrime;
};

class Policy {
  int states,actions;
	private:
		// State states[NumStates][actions][LinkedListUpToNumStates, totNum];
  		// int transitionCount[16][ACTION_SIZE][2];
  int ***transitionCount;  // [# of states][ACTION_SIZE][2];


		//vector<State> **states; //contains model of states. Expandable
		// map (state, Transition);
	public:
		Policy();
  Policy(int states, int actions);
		~Policy();
		virtual Action getAction(State s);
		virtual void Update(StateTransition sas);
		//virtual void Update(StateTransition *sas);
};

#endif
