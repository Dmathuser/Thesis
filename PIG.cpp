#include "PIG.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

PIG_alg::PIG_alg(int seed, int numStates, int numActions):Policy(numStates,numActions)
{
	seed = seed;
	srand(seed); //Might be slow...
}

PIG_alg::~PIG_alg()
{
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
		test = PIG(s,Action(i));

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
	}
	return a;
}

double PIG_alg::PIG(State s, Action a)
{
	double sum = 0;

	//Create new probability vector that contains updated time-step.
	int *transCountNew = new int[N];
	//copy existing vector
	for (int i = 0; i < N; i++) //Include N to update total counter as well.
	{
		transCountNew[i] = transitionCount[s.StateId][a][i];
	}

	transCountNew[N-1] += 1; //Update total step counter for later predictions.

	//Loop over possible resulting states
	for (int sStar = 0; sStar < N-1; sStar++)
	{
		//ThetaHat(sas*) * KL_D(ThetaHat'(s,a) || ThetaHat(s,a))
		transCountNew[sStar] += 1; //Predict as if taken next step
		//double prob = GetProbability(s.StateId, a, sStar);
		double probNew = double(transCountNew[sStar]) / transCountNew[N-1];
		double prob = probNew;
		double KL_div = KL_D(transCountNew, transitionCount[s.StateId][a]);
		double ans = prob * KL_div;
		if (!isnan(ans))
			sum += abs(ans); 
		transCountNew[sStar] -= 1; //Revert step change for next itteration.
	}
	delete[] transCountNew; //Is one Dimentional Vector, should work.
	return sum;
}

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

//NOTE: Assumes that N = States+1;
void PIG_alg::Update(StateTransition sas)
{
	int s = sas.s.StateId;
	int sPrime = sas.sPrime.StateId;
	//int curCount = transitionCount[s][sas.a][sPrime];
	if (N != states+1)
	{
		std::cout << "ERROR: Transition Matrix Dimensionality Changed. (N != states+1)" << std::endl;
		return;
	}
	transitionCount[s][sas.a][sPrime] += 1; //increase counter for sas observed
	transitionCount[s][sas.a][N-1] += 1; //increase counter for total sa.
	if (DEBUG == true)
	{
		std::cout << "New Total for SAS(" << s << "," << sas.a << "," << sPrime
			<< ") " << " is: (" << transitionCount[s][sas.a][sPrime] << "/ " 
			<< transitionCount[s][sas.a][N-1] << ")" << std::endl;
	}
}

// Takes in vector of [integer counts] used to calculate Transition Probabilities (doubles).
double PIG_alg::KL_D(int* Theta, int* ThetaHat)
{
	//Sanity Check Probability Vectors should be same length.
	/*if (N != N_hat)
	{
		std::cout << "ERROR: KL_Divergence Vectors of different Lengths" << std::endl;
		return -1;
	}
	*/
	//Check for empty transition Vector
	if (Theta[N-1] == 0 || ThetaHat[N-1] == 0)
		return std::numeric_limits<double>::infinity();
	double sum = 0;
	// Loop through all possible state transitions given s and a.
	for (int sPrime = 0; sPrime < N-1; sPrime++)
	{
		//Transform integer counts into transition probabilites.
		//TODO?, update to use GetProbability function.
		double transitionProb = double(Theta[sPrime]) / Theta[N-1];
		double transitionProbHat = double(ThetaHat[sPrime]) / ThetaHat[N-1];
		//EQUATION: Theta(sas') * Log_2((Theta(sas')/ThetaHat(sas')))
		double temp = transitionProb/transitionProbHat;
		double ans = transitionProb * log2(temp);
		if (temp != 0 && !isnan(temp))
		{
			if (ans != 0 && !isnan(temp) && !isinf(temp))
			{
				sum += ans;
			}
		}
	}
	return sum;
}

//TODO: Need to update to find sPrime index instead for variable dimensions.
double PIG_alg::GetProbability(StateTransition sas)
{
	return GetProbability(sas.s.StateId, sas.a, sas.sPrime.StateId);
}

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

