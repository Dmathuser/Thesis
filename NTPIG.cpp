#include "NTPIG.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

NTPIG_alg::NTPIG_alg(int seed, int numStates, int numActions):Policy(numStates, numActions)
{
	this->seed = seed;
	rand_seed = seed;
	Initialize();
}

NTPIG_alg::~NTPIG_alg()
{
	// if(Qvalues != NULL)
	// {
	// 	for(int i = 0; i < states; i++)
	// 	{
	// 		// for (int a = 0; a < actions; a++)
	// 		// 	Qvalues[i][a].clear();
	// 		delete[] Qvalues[i];
	// 	}
	// 	delete[] Qvalues;
	// }
}

void NTPIG_alg::Initialize()
{
	transitionCount = NULL;
	// QNvalues.Capacity(states * actions);
	//Loop through transitionCount and update s,a values 
	// Qvalues = new std::vector<double>*[states];
	// for (int i = 0; i < states; i++)
	// {
	// 	Qvalues[i] = new std::vector<double>[actions];
	// 	for (int a = 0; a < actions; a++)
	// 		Qvalues[i][a].reserve(1);
	// }

	// for (int s=0; s < states; s++)
	// 	for (int a=0; a < actions; a++)
	// 	{
	// 		// for (unsigned long n = 0; n < Qvalues[s][a].size(); n++)
	// 		// {
	// 		// 	Qvalues[s][a][n] = 1;
	// 		// }
	// 		transitionCount[s][a].s.StateId = s;
	// 		transitionCount[s][a].s.Noise = 0;
	// 		transitionCount[s][a].a = Action(a);
	// 		transitionCount[s][a].totalCount = 1;
	// 	}
}

Action NTPIG_alg::getAction(State s)
{
	// std::cout << "Start GetAction" << std::endl;
	int randNum = rand_r(&rand_seed)%ACTION_SIZE;
	Action a = Action(randNum);
	double maxReturn = -1;
	double test = -1;
	//Loop over future s' states
	for (int i=0; i < ACTION_SIZE; i++)
	{
		// std::cout << "Loop: " << i << std::endl;
		// std::cout << "State: " << s.StateId << ", Action: " << i << ", Noise: " << s.Noise <<std::endl;
		// test = Qvalues[s.StateId][i][s.Noise];

		//Initialize value if it's not in the data. This prevents deadlocks.
		if (QNValues.find(hash_sa(s,i)) == QNValues.end())
			QNValues[hash_sa(s,i)] = 1;
		test = QNValues[hash_sa(s,i)];
		
		if (test > maxReturn)
		{
			a = Action(i);
			maxReturn = test;
		}
	}
	// std::cout << "getAction returned: " << a << std::endl;
	return a;
}

void NTPIG_alg::Update(StateTransition sas)
{
	
	//std::cout << "sas = {{" << sas.s.StateId << ", " << sas.s.Noise << "}, " << sas.a << ", {" << sas.sPrime.StateId << ", " << sas.sPrime.Noise << "}}" << std::endl;
	//Create new Transition vector assuming (s->s', a)
	StateTransitionVector *transCountNew = new StateTransitionVector;

	// if(!copy_StateTransitionVector(&transitionCount[sas.s.StateId][sas.a], transCountNew))
	//Error messages for hash miss are not nullptr, so need seperate error checking instead.
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
	else if(!copy_StateTransitionVector(&transitionMemory[hash_sa(sas.s, sas.a)], transCountNew))
	{
		std::cout << "ERROR: Could not copy transition vector" << std::endl;
	}

	//std::cout << "transCountNew = {{" << transCountNew->s.StateId << ", " << transCountNew->s.Noise << "}, " << transCountNew->a << "}" << std::endl;

	//Increment counters to mimic state transition s->s'
	//transCountNew->totalCount += 1;
	
	if (transCountNew == nullptr)
		std::cout << "transCountNew is nullptr" << std::endl;

	UpdateStateTransitionVectorTransitions(transCountNew, sas);
	// double Old_KL_Div = KL_D(transCountNew, &transitionCount[sas.s.StateId][sas.a]);
	double Old_KL_Div = KL_D(transCountNew, &transitionMemory[hash_sa(sas.s, sas.a)]);

	//Update Transitions to include new sas transition.
	UpdateTransitions(sas);

	//Calculate KL_Div[(s->s',a) || (s,a,s')] for next timestep
	// if(!copy_StateTransitionVector(&transitionCount[sas.s.StateId][sas.a], transCountNew))
	if(!copy_StateTransitionVector(&transitionMemory[hash_sa(sas.s, sas.a)], transCountNew))
	{
		std::cout << "ERROR: Could not copy transition vector" << std::endl;
	}

	//Increment counters to mimic state transition s->s'
	UpdateStateTransitionVectorTransitions(transCountNew, sas);

	// double New_KL_Div = KL_D(transCountNew, &transitionCount[sas.s.StateId][sas.a]);
	double New_KL_Div = KL_D(transCountNew, &transitionMemory[hash_sa(sas.s, sas.a)]);

	//Calculate Reward
	if (isinf(Old_KL_Div) || isnan(Old_KL_Div))
		Old_KL_Div = 0;
	if (isinf(New_KL_Div) || isnan(New_KL_Div))
		New_KL_Div = 0;

	double reward = New_KL_Div - Old_KL_Div; 
	if (reward < 0)
		reward = 0;

	//Update Q values using intrinsic reward
	// double OldQ = Qvalues[sas.s.StateId][sas.a][sas.s.Noise];
	// double NewQ = Qvalues[sas.sPrime.StateId][getAction(sas.sPrime)][sas.sPrime.Noise];
	double OldQ = QNValues[hash_sa(sas.s, sas.a)];
	double NewQ = QNValues[hash_sa(sas.s, sas.a)];

	//Normalize Qvalues to avoid infinities
	if (isinf(OldQ) || isnan(OldQ))
		OldQ = 0;
	if (isinf(NewQ) || isnan(NewQ))
		NewQ = 0;
	
	OldQ = OldQ + alpha*(reward + gamma*NewQ - OldQ);

	// Qvalues[sas.s.StateId][sas.a][sas.s.Noise] = OldQ;
	QNValues[hash_sa(sas.s, sas.a)] = OldQ;
	delete transCountNew;
}

void NTPIG_alg::UpdateStateTransitionVectorTransitions(StateTransitionVector *Theta, StateTransition sas)
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

void NTPIG_alg::UpdateTransitions(StateTransition sas)
{
	// int s = sas.s.StateId;
	StateCounter *tempStateCounter = nullptr;
	
	// if (findStateCounterIndex(&transitionCount[s][sas.a].sPrime, &sas.sPrime, &tempStateCounter))
	if (findStateCounterIndex(&transitionMemory[hash_sa(sas.s,sas.a)].sPrime, &sas.sPrime, &tempStateCounter))
		tempStateCounter->count += 1;
	else //Add observed transition to known transitions if it wasn't there.
	{
		tempStateCounter = new StateCounter;
		// transitionCount[s][sas.a].sPrime.push_back(*tempStateCounter);
		// transitionCount[s][sas.a].sPrime.back().count += 1;
		transitionMemory[hash_sa(sas.s,sas.a)].sPrime.push_back(*tempStateCounter);
		transitionMemory[hash_sa(sas.s,sas.a)].sPrime.back().count += 1;
		//Free up pointer
		delete tempStateCounter;

		// tempStateCounter = &transitionCount[s][sas.a].sPrime.back(); 
		tempStateCounter = &transitionMemory[hash_sa(sas.s,sas.a)].sPrime.back(); 
		State ObservedState = {sas.sPrime.StateId, sas.sPrime.Noise};
		copy_State(&ObservedState, &tempStateCounter->s);
	}
	// transitionCount[s][sas.a].totalCount += 1;
	transitionMemory[hash_sa(sas.s,sas.a)].totalCount += 1;
}

double NTPIG_alg::KL_D(StateTransitionVector *Theta, StateTransitionVector *ThetaHat)
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

void NTPIG_alg::PrintTransitionVectors(StateTransitionVector *Theta, State s, Action a)
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

void NTPIG_alg::PrintTransitionVectors(State s, Action a)
{
	std::cout << "Vector of counters: ";
	// For Each vector element, Print (stateId, Noise, Count)
	// for (int i = 0; i < int(transitionCount[s.StateId][a].sPrime.size()); i++)
	for (int i = 0; i < int(transitionMemory[hash_sa(s, a)].sPrime.size()); i++)
		std::cout << "(" << transitionMemory[hash_sa(s, a)].sPrime[i].s.StateId << ", " << transitionCount[s.StateId][a].sPrime[i].s.Noise << ", " << transitionCount[s.StateId][a].sPrime[i].count << ")" << ", ";
		// std::cout << "(" << transitionCount[s.StateId][a].sPrime[i].s.StateId << ", " << transitionCount[s.StateId][a].sPrime[i].s.Noise << ", " << transitionCount[s.StateId][a].sPrime[i].count << ")" << ", ";
		//std::cout << transitionCount[s.StateId][a].sPrime[i].count << ", ";
	std::cout << std::endl; 
	std::cout << "State, Noise, Action: (" << s.StateId << ", "
		<< s.Noise << ", " << a << ")" << std::endl;
}


double NTPIG_alg::GetProbability(StateTransitionVector sas, StateCounter s)
{
	return GetProbability(s.count, sas.totalCount);
}

double NTPIG_alg::GetProbability(StateCounter s, int totalCount)
{
	return GetProbability(s.count, totalCount);
}

double NTPIG_alg::GetProbability(StateTransitionVector *sas, int stateIndex)
{
	return GetProbability(sas->sPrime[stateIndex], sas->totalCount);
}

double NTPIG_alg::GetProbability(int visitCount, int totalCount)
{
	return double(visitCount)/totalCount;
}

