#include "NPIG.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

NPIG_alg::NPIG_alg(int seed, int numStates, int numActions):Policy(numStates,numActions)
{
	this->seed = seed;
	rand_seed = seed; //Might be slow...
	Initialize(); //Currently handled in policy class
}

NPIG_alg::~NPIG_alg()
{
}

void NPIG_alg::Initialize()
{
	transitionCount = NULL;
	// //Loop through transitionMemory and update s,a values 
	// for (int s=0; s < states; s++)
	// 	for (int a=0; a < actions; a++)
	// 	{
	// 		transitionMemory[hash_sa(s,a)].s.StateId = s;
	// 		transitionMemory[hash_sa(s,a)].s.Noise = 0;
	// 		transitionMemory[hash_sa(s,a)].a = Action(a);
	// 		transitionMemory[hash_sa(s,a)].totalCount = 1;
	// 	}
}

Action NPIG_alg::getAction(State s)
{
	int randNum = rand_r(&rand_seed)%ACTION_SIZE;
	Action a = Action(randNum);
	double maxReturn = 0;
	double test = 0;
	//Loop over possible actions
	for (int i = 0; i < ACTION_SIZE; i++)
	{
		// Maximize NPIG return.
		test = PIG(s,Action(i));
		if (test < 0)
			std::cout << "WARNING: Negative NPIG value returned." << std::endl;

		if (test > maxReturn) //update if better action is found.
		{
			a = Action(i);
			maxReturn = test;
		}
	}
	return a;
}

double NPIG_alg::PIG(State s, Action a)
{
	double sum = 0;
	//Create new probability vector that contains updated time-step.
	StateTransitionVector *transCountNew = new StateTransitionVector;

	if (transitionMemory.find(hash_sa(s, a)) == transitionMemory.end()) 
	{
		//std::cout << "Creating Vector Memory" << std::endl;
		transCountNew->s.StateId = s.StateId;
		transCountNew->s.Noise = s.Noise;
		transCountNew->a = a;
		transCountNew->totalCount = 1;
		copy_StateTransitionVector(transCountNew, &transitionMemory[hash_sa(s, a)]);
	// 		transitionCount[s][a].s.StateId = s;
	// 		transitionCount[s][a].s.Noise = 0;
	// 		transitionCount[s][a].a = Action(a);
	// 		transitionCount[s][a].totalCount = 1;
	}


	std::vector<StateCounter> *sPrimeVector;
	sPrimeVector = &transitionMemory[hash_sa(s,a)].sPrime;
	int vectSize = sPrimeVector->size(); 

	//copy existing vector
	if(!copy_StateTransitionVector(&transitionMemory[hash_sa(s,a)], transCountNew))
	{
		std::cout << "ERROR: Could not copy transition vector" << std::endl;
		delete transCountNew;
		return 0;
	}
	
	//Update total counter for later predictions
	transCountNew->totalCount += 1;

	//Loop over possible resulting states
	for (int sStar = 0; sStar < vectSize; sStar++)
	{
		//Increment agent counter as if agent has taken the next step
		transCountNew->sPrime[sStar].count += 1; 
		double prob = GetProbability(*transCountNew, sStar);

		double KL_div = KL_D(transCountNew, &transitionMemory[hash_sa(s,a)]);
		double ans = prob * KL_div;
		if (!isnan(ans))
			sum += abs(ans); 

		//Decrement agent counter to prep for next itteration.
		transCountNew->sPrime[sStar].count -= 1; 
	}
	//If KL_div is never calculated, Default to EXPLORE (inf)
	delete transCountNew; //Is one Dimentional Vector, should work.
	if (vectSize == 0)
		return std::numeric_limits<double>::infinity();
	return sum;
}

void NPIG_alg::PrintTransitionVectors(State s, Action a)
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

//NOTE: Assumes that N = States+1;
void NPIG_alg::Update(StateTransition sas)
{
	int s = sas.s.StateId;
	StateCounter *tempStateCounter = nullptr;
	//Update counter for transition observed
	if (transitionMemory.find(hash_sa(sas.s, sas.a)) == transitionMemory.end()) 
	{
		//std::cout << "Entered" << std::endl;
		StateTransitionVector *transCountNew = new StateTransitionVector;
		transCountNew->s.StateId = sas.s.StateId;
		transCountNew->s.Noise = sas.s.Noise;
		transCountNew->a = sas.a;
		transCountNew->totalCount = 1;
		copy_StateTransitionVector(transCountNew, &transitionMemory[hash_sa(sas.s, sas.a)]);
		//std::cout << "transCountNew = {{" << transCountNew->s.StateId << ", " << transCountNew->s.Noise << "}, " << transCountNew->a << "}" << std::endl;
		delete transCountNew;
	}

	//Cheats because it doesn't handle noise in the stored state transition inside "transitionMemory"
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

	//increase counter for total number of observed transitions.
	transitionMemory[hash_sa(sas.s,sas.a)].totalCount += 1;
	if (DEBUG == true)
	{
		PrintTransitionVectors(sas.s, sas.a);
		std::cout << std::endl;
		std::cout << std::endl << std::endl << "(s.Id, sNoise, a, s'Id, s'Noise): (" << s << ", " << sas.s.Noise << ", " << sas.a << ", " << sas.sPrime.StateId << ", " << sas.sPrime.Noise << ")" << std::endl;
		std::cout << "New observation for SAS(" << s << "," << sas.a 
			<< "," << sas.sPrime.StateId	<< ") :" << tempStateCounter->count << std::endl;
	}
}

// Takes in vector of [integer counts] used to calculate Transition Probabilities (doubles).
	//If prob in both Theta and ThetaHat
		//Calculate prob and return
	//If prob in ThetaHat but not Theta
		//Return 0;
	//If prob in Theta but not ThetaHat
		//Return inf;
	//If prob in neither Theta or ThetaHat
		//Return 0;
double NPIG_alg::KL_D(StateTransitionVector *Theta, StateTransitionVector *ThetaHat)
{
	//Check for empty transition Vector
	if (Theta->sPrime.size() == 0 || ThetaHat->sPrime.size() == 0)
		return std::numeric_limits<double>::infinity();
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
		ThetaProb = GetProbability(*Theta, sPrime);
		ThetaHatProb = GetProbability(*ThetaHat, *ThetaHatState);

		//EQUATION: Theta(sas') * Log_2((Theta(sas')/ThetaHat(sas')))
		double ans = ThetaProb * log2(ThetaProb / ThetaHatProb);
		if (ans != 0 && !isnan(ans) && !isinf(ans))
			sum += ans;
	}
	return sum;
}

double NPIG_alg::GetProbability(StateTransition sas)
{
	int visitCount = 0;
	int totalCount = transitionMemory[hash_sa(sas.s,sas.a)].totalCount;

	std::vector<StateCounter> *sPrimeVector;
	sPrimeVector = &transitionMemory[hash_sa(sas.s,sas.a)].sPrime;
	int vectCount = sPrimeVector->size();

	//find correct state in state vector, then grab count
	for (int i = 0; i < vectCount; i++)
	{
		if(isStateEqual(&sPrimeVector->at(i).s, &sas.s))
		{
			visitCount = sPrimeVector->at(i).count;
			break;
		}
	}
	
	return GetProbability(visitCount, totalCount);
}

double NPIG_alg::GetProbability(StateTransitionVector sas, StateCounter s)
{
	return GetProbability(s.count, sas.totalCount);
}

double NPIG_alg::GetProbability(StateCounter s, int totalCount)
{
	return GetProbability(s.count, totalCount);
}

double NPIG_alg::GetProbability(StateTransitionVector sas, int stateIndex)
{
	return GetProbability(sas.sPrime[stateIndex], sas.totalCount);
}

double NPIG_alg::GetProbability(int visitCount, int totalCount)
{
	return double(visitCount)/totalCount;
}

