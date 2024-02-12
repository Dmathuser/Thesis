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

/*
  Sometimes we need to dynamically allocate a 2-D array with small
  width.  The standard way to allocate 2-d array is an array of
  pointers, one for each row, then an array of elements for each row.
  But creating a type for the colums is more efficient in space and
  time if there are only a few columns, and the number of columns is
  static.
*/
template <typename T,int size> struct basearray{
  T items[size];  // array of items
  T& operator [](int idx)  // overload subscription operator
  {
    return items[idx];
  }
};

class Policy {
  int states,actions;
	private:
		// State states[NumStates][actions][LinkedListUpToNumStates, totNum];
  // int transitionCount[16][ACTION_SIZE][2];
  basearray<int,2> **transitionCount;  // [# of states][ACTION_SIZE][2];


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
