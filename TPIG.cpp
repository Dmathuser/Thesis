#include "TPIG.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

TPIG_alg::TPIG_alg(int seed, int numStates, int numActions):Policy(numStates, numActions)
{
	this->seed = seed;
	srand(seed);
	Initialize();
}

TPIG_alg::~TPIG_alg()
{
	if(Qvalues != NULL)
	{
		for(int i = 0; i < states; i++)
			delete[] Qvalues[i];
		delete[] Qvalues;
	}
}

void TPIG_alg::Initialize()
{
	//Loop through transitionCount and update s,a values 
	Qvalues = new double*[states];
	for (int i = 0; i < states; i++)
		Qvalues[i] = new double[actions];

	for (int s=0; s < states; s++)
		for (int a=0; a < actions; a++)
		{
			Qvalues[s][a] = 1;
			transitionCount[s][a].s.StateId = s;
			transitionCount[s][a].s.Noise = 0;
			transitionCount[s][a].a = Action(a);
			transitionCount[s][a].totalCount = 1;
		}
}

Action TPIG_alg::getAction(State s)
{
	int randNum = rand()%ACTION_SIZE;
	Action a = Action(randNum);
	double maxReturn = -1;
	double test = -1;
	//Loop over future s' states
	for (int i=0; i < ACTION_SIZE; i++)
	{
		test = Qvalues[s.StateId][i];
		
		if (test > maxReturn)
		{
			a = Action(i);
			maxReturn = test;
		}
	}
	return a;
}

void TPIG_alg::Update(StateTransition sas)
{
	//Create new Transition vector assuming (s->s', a)
	StateTransitionVector *transCountNew = new StateTransitionVector;
	if(!copy_StateTransitionVector(&transitionCount[sas.s.StateId][sas.a], transCountNew))
	{
		std::cout << "ERROR: Could not copy transition vector" << std::endl;
	}

	//Increment counters to mimic state transition s->s'
	//transCountNew->totalCount += 1;
	
	if (transCountNew == nullptr)
		std::cout << "transCountNew is nullptr" << std::endl;

	UpdateStateTransitionVectorTransitions(transCountNew, sas);
	double Old_KL_Div = KL_D(transCountNew, &transitionCount[sas.s.StateId][sas.a]);

	//Update Transitions to include new sas transition.
	UpdateTransitions(sas);

	//Calculate KL_Div[(s->s',a) || (s,a,s')] for next timestep
	if(!copy_StateTransitionVector(&transitionCount[sas.s.StateId][sas.a], transCountNew))
	{
		std::cout << "ERROR: Could not copy transition vector" << std::endl;
	}

	//Increment counters to mimic state transition s->s'
	UpdateStateTransitionVectorTransitions(transCountNew, sas);

	double New_KL_Div = KL_D(transCountNew, &transitionCount[sas.s.StateId][sas.a]);

	//Calculate Reward
	if (isinf(Old_KL_Div) || isnan(Old_KL_Div))
		Old_KL_Div = 0;
	if (isinf(New_KL_Div) || isnan(New_KL_Div))
		New_KL_Div = 0;

	double reward = New_KL_Div - Old_KL_Div; 
	if (reward < 0)
		reward = 0;

	//Update Q values using intrinsic reward
	double OldQ = Qvalues[sas.s.StateId][sas.a];
	double NewQ = Qvalues[sas.sPrime.StateId][getAction(sas.sPrime)];

	//Normalize Qvalues to avoid infinities
	if (isinf(OldQ) || isnan(OldQ))
		OldQ = 0;
	if (isinf(NewQ) || isnan(NewQ))
		NewQ = 0;
	
	OldQ = OldQ + alpha*(reward + gamma*NewQ - OldQ);

	Qvalues[sas.s.StateId][sas.a] = OldQ;
	delete transCountNew;
}

void TPIG_alg::UpdateStateTransitionVectorTransitions(StateTransitionVector *Theta, StateTransition sas)
{
	StateCounter *tempStateCounter = nullptr;
	
	if (findStateCounterIndex(&Theta->sPrime, &sas.sPrime, &tempStateCounter))
		tempStateCounter->count += 1;
	else //Add observed transition to known transitions if it wasn't there.
	{
		tempStateCounter = new StateCounter;
		Theta->sPrime.push_back(*tempStateCounter);
		Theta->sPrime.back().count += 1;
		//Free up pointer
		delete tempStateCounter;

		tempStateCounter = &Theta->sPrime.back(); 
		State ObservedState = {sas.sPrime.StateId, sas.sPrime.Noise};
		copy_State(&ObservedState, &tempStateCounter->s);
	}
	Theta->totalCount += 1;
}

void TPIG_alg::UpdateTransitions(StateTransition sas)
{
	int s = sas.s.StateId;
	StateCounter *tempStateCounter = nullptr;
	
	if (findStateCounterIndex(&transitionCount[s][sas.a].sPrime, &sas.sPrime, &tempStateCounter))
		tempStateCounter->count += 1;
	else //Add observed transition to known transitions if it wasn't there.
	{
		tempStateCounter = new StateCounter;
		transitionCount[s][sas.a].sPrime.push_back(*tempStateCounter);
		transitionCount[s][sas.a].sPrime.back().count += 1;
		//Free up pointer
		delete tempStateCounter;

		tempStateCounter = &transitionCount[s][sas.a].sPrime.back(); 
		State ObservedState = {sas.sPrime.StateId, sas.sPrime.Noise};
		copy_State(&ObservedState, &tempStateCounter->s);
	}
	transitionCount[s][sas.a].totalCount += 1;
}

double TPIG_alg::KL_D(StateTransitionVector *Theta, StateTransitionVector *ThetaHat)
{
	//Check for empty transition Vector
	double sum = 0;
	int vectSize = Theta->sPrime.size();

	double ThetaProb;
	double ThetaHatProb;
	State *curState;
	StateCounter *ThetaHatState = nullptr;

	//Itterate through Theta
		//No match means inf, Match means calculate.
	// Loop through all observed state transitions given s and a.
	for (int sPrime = 0; sPrime < vectSize; sPrime++)
	{
		if (Theta == nullptr)
			std::cout << "ERROR: TransitionVector element is nullptr";
		curState = &Theta->sPrime[sPrime].s;
		//Check if theta transition is found in thetahat, exit if no	
		if (!findStateCounterIndex(&ThetaHat->sPrime, curState, &ThetaHatState))
			return std::numeric_limits<double>::infinity();
		//calculate and add to sum.
		//Transform integer counts into transition probabilites.
		ThetaProb = GetProbability(Theta, sPrime);
		ThetaHatProb = GetProbability(*ThetaHat, *ThetaHatState);

		//EQUATION: Theta(sas') * Log_2((Theta(sas')/ThetaHat(sas')))
		double ans = ThetaProb * log2(ThetaProb / ThetaHatProb);
		if (ans != 0 && !isnan(ans) && !isinf(ans))
			sum += ans;
	}
	return sum;
}

void TPIG_alg::PrintTransitionVectors(StateTransitionVector *Theta, State s, Action a)
{
	std::cout << "Vector of counters: ";
	// For Each vector element, Print (stateId, Noise, Count)
	for (int i = 0; i < int(Theta->sPrime.size()); i++)
		std::cout << "(" << Theta->sPrime[i].s.StateId << ", " << Theta->sPrime[i].s.Noise << ", " << Theta->sPrime[i].count << ")" << ", ";
		//std::cout << transitionCount[s.StateId][a].sPrime[i].count << ", ";
	std::cout << std::endl; 
	//std::cout << "State, Noise, Action: (" << Theta->s.StateId << ", "
		//<< Theta->s.Noise << ", " << Theta->a << ")" << std::endl;
}

void TPIG_alg::PrintTransitionVectors(State s, Action a)
{
	std::cout << "Vector of counters: ";
	// For Each vector element, Print (stateId, Noise, Count)
	for (int i = 0; i < int(transitionCount[s.StateId][a].sPrime.size()); i++)
		std::cout << "(" << transitionCount[s.StateId][a].sPrime[i].s.StateId << ", " << transitionCount[s.StateId][a].sPrime[i].s.Noise << ", " << transitionCount[s.StateId][a].sPrime[i].count << ")" << ", ";
		//std::cout << transitionCount[s.StateId][a].sPrime[i].count << ", ";
	std::cout << std::endl; 
	std::cout << "State, Noise, Action: (" << s.StateId << ", "
		<< s.Noise << ", " << a << ")" << std::endl;
}


double TPIG_alg::GetProbability(StateTransitionVector sas, StateCounter s)
{
	return GetProbability(s.count, sas.totalCount);
}

double TPIG_alg::GetProbability(StateCounter s, int totalCount)
{
	return GetProbability(s.count, totalCount);
}

double TPIG_alg::GetProbability(StateTransitionVector *sas, int stateIndex)
{
	return GetProbability(sas->sPrime[stateIndex], sas->totalCount);
}

double TPIG_alg::GetProbability(int visitCount, int totalCount)
{
	return double(visitCount)/totalCount;
}

