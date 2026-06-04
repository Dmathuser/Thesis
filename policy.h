#ifndef _POLICY_H
#define _POLICY_H
#include "enums.h"
#include "structs.h"
#include <unordered_map>

class Policy {
	protected:
  int states,actions;
	///*const*/ int N = 2; //Size of third dimension of transition vector.
	const int MAX_TRANSITION_STATES = 2; //A useless const currently
		// State states[NumStates][actions][LinkedListUpToNumStates, totNum];
  StateTransitionVector **transitionCount;  
  std::unordered_map<unsigned long, StateTransitionVector> transitionMemory; 
		// [# of states][ACTION_SIZE];
		//vector<State> **states; //contains model of states. Expandable
	void Initialize();
	public:
		Policy();
		Policy(int states, int actions);
		~Policy();
  	int getNumStates(){return states;}
	unsigned long hash_sa(State state, int action);
  	//int getN(){return N;}
  	StateTransitionVector** getTransitionCount(){return transitionCount;}
  	std::unordered_map<unsigned long, StateTransitionVector> getTransitionMemory(){return transitionMemory;}
		virtual Action getAction(State s);
		virtual void Update(StateTransition sas);
		//virtual void Update(StateTransition *sas);
};

#endif
