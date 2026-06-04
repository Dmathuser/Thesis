#include "NETPIG.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

NETPIG_alg::NETPIG_alg(int seed, int Epsilon, int numStates, int numActions):Policy(numStates, numActions)
{
	this->seed = seed;
	rand_seed = seed;
	EPSILON = Epsilon;
	Initialize();
}

NETPIG_alg::~NETPIG_alg()
{
	// if(qvalues != NULL)
	// {
	// 	for(int i = 0; i < states; i++)
	// 		delete[] Qvalues[i];
	// 	delete[] Qvalues;
	// }
}

void NETPIG_alg::Initialize()
{
	transitionCount = NULL;
	//Loop through transitionCount and update s,a values 
	// Qvalues = new double*[states];
	// for (int i = 0; i < states; i++)
	// 	Qvalues[i] = new double[actions];

	// for (int s=0; s < states; s++)
	// 	for (int a=0; a < actions; a++)
	// 	{
	// 		// Qvalues[s][a] = 1;
	// 		transitionCount[s][a].s.StateId = s;
	// 		transitionCount[s][a].s.Noise = 0;
	// 		transitionCount[s][a].a = Action(a);
	// 		transitionCount[s][a].totalCount = 1;
	// 	}
}

Action NETPIG_alg::getAction(State s)
{
	int randNum = rand_r(&rand_seed)%ACTION_SIZE;
	Action a = Action(randNum);
	double maxReturn = -1;
	double test = -1;
	//Epsilon Greedy
	int random = rand_r(&rand_seed)%100;
	if (random < EPSILON)
		return Action(rand_r(&rand_seed)%ACTION_SIZE);
	//Loop over future s' states
	for (int i=0; i < ACTION_SIZE; i++)
	{
		if (QNValues.find(hash_sa(s,i)) == QNValues.end())
			QNValues[hash_sa(s,i)] = 1;
		test = QNValues[hash_sa(s,i)];
		
		if (test > maxReturn)
		{
			a = Action(i);
			maxReturn = test;
		}
	}
	//Have an Epsilon % chance of picking a random non-optimal action.
	/*
	randNum = rand_r(&rand_seed)%100;
	if (randNum < EPSILON)
	{
		//Get a random offset for the action. 
		//randNum = rand_r(&rand_seed)%(ACTION_SIZE-1);
		randNum = rand() % ACTION_SIZE;
		//Pick a different action randomly based on the offset
		//randNum = (randNum+int(a)) % ACTION_SIZE;
		a = Action(randNum);
	}
	*/
	return a;
}

void NETPIG_alg::Update(StateTransition sas)
{
	//Create new Transition vector assuming (s->s', a)
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
	else if(!copy_StateTransitionVector(&transitionMemory[hash_sa(sas.s,sas.a)], transCountNew))
	{
		std::cout << "ERROR: Could not copy transition vector" << std::endl;
	}

	//Increment counters to mimic state transition s->s'
	
	if (transCountNew == nullptr)
		std::cout << "transCountNew is nullptr" << std::endl;

	UpdateStateTransitionVectorTransitions(transCountNew, sas);
	double Old_KL_Div = KL_D(transCountNew, &transitionMemory[hash_sa(sas.s,sas.a)]);

	//Update Transitions to include new sas transition.
	UpdateTransitions(sas);

	//Calculate KL_Div[(s->s',a) || (s,a,s')] for next timestep
	if(!copy_StateTransitionVector(&transitionMemory[hash_sa(sas.s,sas.a)], transCountNew))
	{
		std::cout << "ERROR: Could not copy transition vector" << std::endl;
	}
	//Increment counters to mimic state transition s->s'
	UpdateStateTransitionVectorTransitions(transCountNew, sas);
	double New_KL_Div = KL_D(transCountNew, &transitionMemory[hash_sa(sas.s,sas.a)]);

	//Calculate Reward
	if (isinf(Old_KL_Div) || isnan(Old_KL_Div))
		Old_KL_Div = 0;
	if (isinf(New_KL_Div) || isnan(New_KL_Div))
		New_KL_Div = 0;
	double reward = New_KL_Div - Old_KL_Div; 
	if (reward < 0)
		reward = 0;
	//Update Q values using intrinsic reward
	double OldQ = QNValues[hash_sa(sas.s,sas.a)];
	double NewQ = QNValues[hash_sa(sas.sPrime, getAction(sas.sPrime))];

	//Normalize Qvalues to avoid infinities
	if (isinf(OldQ) || isnan(OldQ))
		OldQ = 0;
	if (isinf(NewQ) || isnan(NewQ))
		NewQ = 0;
	
	OldQ = OldQ + alpha*(reward + gamma*NewQ - OldQ);

	QNValues[hash_sa(sas.s,sas.a)] = OldQ;
	delete transCountNew;
}

void NETPIG_alg::UpdateStateTransitionVectorTransitions(StateTransitionVector *Theta, StateTransition sas)
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

void NETPIG_alg::UpdateTransitions(StateTransition sas)
{
	int s = sas.s.StateId;
	StateCounter *tempStateCounter = nullptr;
	
	if (findStateCounterIndex(&transitionMemory[hash_sa(sas.s,sas.a)].sPrime, &sas.sPrime, &tempStateCounter))
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
	transitionMemory[hash_sa(sas.s,sas.a)].totalCount += 1;
}

double NETPIG_alg::KL_D(StateTransitionVector *Theta, StateTransitionVector *ThetaHat)
{
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

void NETPIG_alg::PrintTransitionVectors(StateTransitionVector *Theta, State s, Action a)
{
	std::cout << "Vector of counters: ";
	// For Each vector element, Print (stateId, Noise, Count)
	for (int i = 0; i < int(Theta->sPrime.size()); i++)
		std::cout << "(" << Theta->sPrime[i].s.StateId << ", " << Theta->sPrime[i].s.Noise << ", " << Theta->sPrime[i].count << ")" << ", ";
		//std::cout << transitionMemory[hash_sa(s,a)].sPrime[i].count << ", ";
	std::cout << std::endl; 
	//std::cout << "State, Noise, Action: (" << Theta->s.StateId << ", "
		//<< Theta->s.Noise << ", " << Theta->a << ")" << std::endl;
}

void NETPIG_alg::PrintTransitionVectors(State s, Action a)
{
	std::cout << "Vector of counters: ";
	// For Each vector element, Print (stateId, Noise, Count)
	for (int i = 0; i < int(transitionMemory[hash_sa(s,a)].sPrime.size()); i++)
		std::cout << "(" << transitionMemory[hash_sa(s,a)].sPrime[i].s.StateId << ", " << transitionMemory[hash_sa(s,a)].sPrime[i].s.Noise << ", " << transitionMemory[hash_sa(s,a)].sPrime[i].count << ")" << ", ";
		//std::cout << transitionMemory[hash_sa(s,a)].sPrime[i].count << ", ";
	std::cout << std::endl; 
	std::cout << "State, Noise, Action: (" << s.StateId << ", "
		<< s.Noise << ", " << a << ")" << std::endl;
}


double NETPIG_alg::GetProbability(StateTransitionVector sas, StateCounter s)
{
	return GetProbability(s.count, sas.totalCount);
}

double NETPIG_alg::GetProbability(StateCounter s, int totalCount)
{
	return GetProbability(s.count, totalCount);
}

double NETPIG_alg::GetProbability(StateTransitionVector *sas, int stateIndex)
{
	return GetProbability(sas->sPrime[stateIndex], sas->totalCount);
}

double NETPIG_alg::GetProbability(int visitCount, int totalCount)
{
	return double(visitCount)/totalCount;
}

