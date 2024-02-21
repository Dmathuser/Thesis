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
	protected:
  int states,actions;
	/*const*/ int N = 2; //Size of third dimension of transition vector.
	const int MAX_TRANSITION_STATES = 2; //A useless const currently
		// State states[NumStates][actions][LinkedListUpToNumStates, totNum];
  		// int transitionCount[16][ACTION_SIZE][N];
  int ***transitionCount;  // [# of states][ACTION_SIZE][N];


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
