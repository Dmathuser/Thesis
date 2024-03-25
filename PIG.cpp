#include "PIG.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

PIG_alg::PIG_alg(int seed, int numStates, int numActions):Policy(numStates,numActions)
{
	this->seed = seed;
	srand(seed); //Might be slow...
	Initialize();
}

PIG_alg::~PIG_alg()
{
}
void PIG_alg::Initialize()
{
	//Loop through transitionCount and update s,a values 
	for (int s=0; s < states; s++)
		for (int a=0; a < actions; a++)
		{
			transitionCount[s][a].s.StateId = s;
			transitionCount[s][a].s.Noise = 0;
			transitionCount[s][a].a = Action(a);
			transitionCount[s][a].totalCount = Action(a);
		}
}

Action PIG_alg::getAction(State s)
{
	int randNum = rand()%ACTION_SIZE;
	Action a = Action(randNum);
	//std::cout << "Policy Size is: [" << states << ", " << actions
		//<< ", " << N << "]" << std::endl;
	double maxReturn = -1;
	double test = -1;
	//Loop over possible actions
	for (int i = 0; i < ACTION_SIZE; i++)
	{
		//std::cout << "Best action is now: " << a << std::endl;
		// Maximize PIG return.
		//std::cout << std::endl << "Pre GetAction - ";
		//PrintTransitionVectors(s, a);
		test = PIG(s,Action(i));

		//std::cout << std::endl << "Mid GetAction - ";
		//PrintTransitionVectors(s, a);
		//if (test != 0)
		//std::cout << "PIG(" << s.StateId << "," << i << ") returned: " << test << std::endl;
		if (test > maxReturn) //update if better action is found.
		{
			a = Action(i);
			//std::cout << "Best action is now: " << a 
				//<< " at a PIG(s,a) score of: " << test << " over " 
				//<< maxReturn << std::endl;
			maxReturn = test;
		}
		//std::cout << std::endl << "Post GetAction - ";
		//PrintTransitionVectors(s, a);
	}
	//std::cout << std::endl << "GetAction - ";
	//PrintTransitionVectors(s, a);
	return a;
}

double PIG_alg::PIG(State s, Action a)
{
	double sum = 0;
	// N is Size of third dimension of transition vector.
	// Intuitively, N is the number of s' found from a given [s,a] pair.

	std::vector<StateCounter> *sPrimeVector;
	sPrimeVector = &transitionCount[s.StateId][a].sPrime;
	int vectSize = sPrimeVector->size(); 

	//Create new probability vector that contains updated time-step.
	StateTransitionVector *transCountNew = new StateTransitionVector;
	//copy existing vector
	//std::cout << std::endl << "Pre PIG Copy - ";
	//PrintTransitionVectors(s, a);
	//std::cout << "Source Index - " << &transitionCount[s.StateId][a] << std::endl;
	//std::cout << "Source Action - " << transitionCount[s.StateId][a].a  << std::endl;
	if(!copy_StateTransitionVector(&transitionCount[s.StateId][a], transCountNew))
	{
		std::cout << "ERROR: Could not copy transition vector" << std::endl;
		return 0;
	}
	//std::cout << std::endl << "Post PIG Copy - ";
	//PrintTransitionVectors(s, a);
	
	//Update total counter for later predictions
	transCountNew->totalCount += 1;
	//transCountNew[N-1] += 1; //Update total step counter for later predictions.

	//std::cout << "Vector Size: " << vectSize << std::endl;
	//Loop over possible resulting states
	for (int sStar = 0; sStar < vectSize; sStar++)
	{
		//ThetaHat(sas*) * KL_D(ThetaHat'(s,a) || ThetaHat(s,a))

		//Increment agent counter as if agent has taken the next step
		transCountNew->sPrime[sStar].count += 1; 
		double prob = GetProbability(*transCountNew, sStar);
		//double probNew = double(transCountNew[sStar]) / transCountNew[N-1];
		//double prob = probNew;
		double KL_div = KL_D(transCountNew, &transitionCount[s.StateId][a]);
		double ans = prob * KL_div;
		//std::cout << "Theta sas' , Kl_div, Ans: (" << prob << ", " << KL_div
			//<< ", " << ans << ")" << std::endl;
		if (!isnan(ans))
			sum += abs(ans); 
		//Decrement agent counter to prep for next itteration.
		transCountNew->sPrime[sStar].count -= 1; 
	}
	//If KL_div is never calculated, Default to EXPLORE (inf)
	if (vectSize == 0)
		return std::numeric_limits<double>::infinity();
		//std::cout << "--------------------------------------" << std::endl;
	//delete[] transCountNew; //Is one Dimentional Vector, should work.
	return sum;
}
/*
int* PIG_alg::MakeNewCount(int *curCountVector, int changedCount)
{
	int *transCountNew = new int[N-1];
	//copy existing vector
	for (int i = 0; i < N; i++) //Include N to update total counter as well.
	{
		transCountNew[i] = curCountVector[i];
	}
	// Update as if taking new state transition.
	transCountNew[changedCount] += 1;
	transCountNew[N-1] += 1;
	return transCountNew;
}
*/
void PIG_alg::PrintTransitionVectors(State s, Action a)
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

//NOTE: Assumes that N = States+1;
void PIG_alg::Update(StateTransition sas)
{
	int s = sas.s.StateId;
	int sPrime = sas.sPrime.StateId;
	StateCounter *tempStateCounter = nullptr;
	//int curCount = transitionCount[s][sas.a][sPrime];
	//Update counter for transition observed

	//std::cout << std::endl << std::endl << "(s.Id, sNoise, a, s'Id, s'Noise): (" << s << ", " << sas.s.Noise << ", " << sas.a << ", " << sPrime << ", " << sas.sPrime.Noise << ")" << std::endl;

	//std::cout << "Pre-Update - ";
	//PrintTransitionVectors(sas.s, sas.a);
	
	if (findStateCounterIndex(&transitionCount[s][sas.a].sPrime, &sas.sPrime, &tempStateCounter))
		tempStateCounter->count += 1;
	else //Add observed transition to known transitions if it wasn't there.
	{
		tempStateCounter = new StateCounter;
		State ObservedState = {sPrime,sas.sPrime.Noise};
		transitionCount[s][sas.a].sPrime.push_back(*tempStateCounter);
		transitionCount[s][sas.a].sPrime.back().count += 1;
		tempStateCounter = &transitionCount[s][sas.a].sPrime.back(); 
		copy_State(&ObservedState, &tempStateCounter->s);
		//std::cout << "New Count: " << tempStateCounter->count << std::endl;
		//std::cout << "New Size: " << transitionCount[s][sas.a].sPrime.size() << std::endl;
	}
	//std::cout << "State, Noise: (" << tempStateCounter->s.StateId << ", " << tempStateCounter->s.Noise << ")" << std::endl;

	//std::cout << "Post Update - ";
	//PrintTransitionVectors(sas.s, sas.a);
	//std::cout << std::endl;

	//increase counter for total number of observed transitions.
	transitionCount[s][sas.a].totalCount += 1;
	if (DEBUG == true)
	{
		std::cout << "New observation for SAS(" << s << "," << sas.a 
			<< "," << sPrime	<< ") :" << tempStateCounter->count << std::endl;
	}
}

// Takes in vector of [integer counts] used to calculate Transition Probabilities (doubles).
double PIG_alg::KL_D(StateTransitionVector *Theta, StateTransitionVector *ThetaHat)
{
	//Check for empty transition Vector
	if (Theta->totalCount == 0 || ThetaHat->totalCount == 0)
		return std::numeric_limits<double>::infinity();
	double sum = 0;
	int vectSize = Theta->sPrime.size();
	// Loop through all observed state transitions given s and a.

	//If prob in both Theta and ThetaHat
		//Calculate prob and return
	//If prob in ThetaHat but not Theta
		//Return 0;
	//If prob in Theta but not ThetaHat
		//Return inf;
	//If prob in neither Theta or ThetaHat
		//Return 0;
	
	//Itterate through Theta
		//No match means inf, Match means calculate.

	double ThetaProb;
	double ThetaHatProb;
	State *curState;
	StateCounter *ThetaHatState = nullptr;

	//std::cout << "KL_D Vector Size: " << vectSize << std::endl;
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
		//std::cout << "Successfully Got ThetaHatState:" << ThetaHatState << std::endl;
		ThetaProb = GetProbability(*Theta, sPrime);
		ThetaHatProb = GetProbability(*ThetaHat, *ThetaHatState);

		/*std::cout << "ThetaHatCount, ThetaHatTotalCount: ("
			<< ThetaHatState->count << ", " << ThetaHat->totalCount
			<< ")" << std::endl;
		std::cout << "ThetaProb, ThetaHatProb: (" << ThetaProb << ", "
			<< ThetaHatProb << ")" << std::endl;
		*/
		//EQUATION: Theta(sas') * Log_2((Theta(sas')/ThetaHat(sas')))
		double ans = ThetaProb * log2(ThetaProb / ThetaHatProb);
		if (ans != 0 && !isnan(ans) && !isinf(ans))
			sum += ans;
	}
	return sum;
}

//TODO: Need to update to find sPrime index instead for variable dimensions.
double PIG_alg::GetProbability(StateTransition sas)
{
	int visitCount = 0;
	int totalCount = transitionCount[sas.s.StateId][sas.a].totalCount;

	std::vector<StateCounter> *sPrimeVector;
	sPrimeVector = &transitionCount[sas.s.StateId][sas.a].sPrime;
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

double PIG_alg::GetProbability(StateTransitionVector sas, StateCounter s)
{
	return GetProbability(s.count, sas.totalCount);
}

double PIG_alg::GetProbability(StateCounter s, int totalCount)
{
	return GetProbability(s.count, totalCount);
}

double PIG_alg::GetProbability(StateTransitionVector sas, int stateIndex)
{
	return GetProbability(sas.sPrime[stateIndex], sas.totalCount);
}

double PIG_alg::GetProbability(int visitCount, int totalCount)
{
	return double(visitCount)/totalCount;
}

/*
double PIG_alg::GetProbability(int s, Action a, int sPrime)
{
	double prob = 0;
	//Sanity Check
	if (sPrime >= N)
	{
		std::cout << "ERROR: Future State not in vector" << std::endl;
		return -1;
	}
	//Grab count from [states, actions, states+1] transition count vector.
	int visitCount = 0;
	if (N != (states+1))
	{
		std::cout << "WARNING: State transition vector 3rd dimension different" << std::endl;
		visitCount = transitionCount[s][a][0];
	}
	else
		visitCount = transitionCount[s][a][sPrime];
	int totalCount = transitionCount[s][a][N-1];
	if (totalCount == 0)
	{
		return 0;
	}
	prob = double(visitCount)/totalCount;
	return prob;
}
*/
