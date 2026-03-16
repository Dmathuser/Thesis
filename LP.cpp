#include "LP.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

LP_alg::LP_alg(int seed, int numStates, int numActions):Policy(numStates, numActions)
{
	this->seed = seed;
	rand_seed = seed;
	Initialize();
}

LP_alg::~LP_alg()
{
	if(Qvalues != NULL)
	{
		int i;
		for(i = 0; i < states; i++)
			delete[] Qvalues[i];
		delete[] Qvalues;
	}
}

void LP_alg::Initialize()
{
	int s, a;

	Qvalues = new double*[states];
	for (s = 0; s < states; s++)
		Qvalues[s] = new double[actions];

	for (s = 0; s < states; s++)
		for (a = 0; a < actions; a++)
		{
			Qvalues[s][a] = 1;
			transitionCount[s][a].s.StateId = s;
			transitionCount[s][a].s.Noise = 0;
			transitionCount[s][a].a = Action(a);
			transitionCount[s][a].totalCount = 1;
		}
}

Action LP_alg::getAction(State s)
{
	int randNum = rand_r(&rand_seed)%ACTION_SIZE;
	Action a = Action(randNum);
	double maxReturn = -1;
	double test = -1;
	int i;

	for (i = 0; i < ACTION_SIZE; i++)
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

void LP_alg::Update(StateTransition sas)
{
	double Old_MSE, New_MSE, reward, OldQ, NewQ;

	/* Create hypothetical updated transition vector */
	StateTransitionVector *transCountNew = new StateTransitionVector;
	if(!copy_StateTransitionVector(&transitionCount[sas.s.StateId][sas.a], transCountNew))
		std::cout << "ERROR: Could not copy transition vector" << std::endl;

	/* Compute MSE before update: how different would model be if we
	   saw this transition? */
	UpdateStateTransitionVectorTransitions(transCountNew, sas);
	Old_MSE = MSE(transCountNew, &transitionCount[sas.s.StateId][sas.a]);

	/* Actually update the model */
	UpdateTransitions(sas);

	/* Compute MSE after update: how different would model be if we
	   saw this transition again? */
	if(!copy_StateTransitionVector(&transitionCount[sas.s.StateId][sas.a], transCountNew))
		std::cout << "ERROR: Could not copy transition vector" << std::endl;

	UpdateStateTransitionVectorTransitions(transCountNew, sas);
	New_MSE = MSE(transCountNew, &transitionCount[sas.s.StateId][sas.a]);

	/* Reward = decrease in prediction error */
	if (isnan(Old_MSE) || isinf(Old_MSE))
		Old_MSE = 0;
	if (isnan(New_MSE) || isinf(New_MSE))
		New_MSE = 0;

	reward = Old_MSE - New_MSE;
	if (reward < 0)
		reward = 0;

	/* Update Q-values */
	OldQ = Qvalues[sas.s.StateId][sas.a];
	NewQ = Qvalues[sas.sPrime.StateId][getAction(sas.sPrime)];

	if (isinf(OldQ) || isnan(OldQ))
		OldQ = 0;
	if (isinf(NewQ) || isnan(NewQ))
		NewQ = 0;

	OldQ = OldQ + alpha*(reward + gamma*NewQ - OldQ);
	Qvalues[sas.s.StateId][sas.a] = OldQ;

	delete transCountNew;
}

/* Mean squared error between two transition probability distributions.
   Computes MSE over all states in Theta's distribution. */
double LP_alg::MSE(StateTransitionVector *Theta, StateTransitionVector *ThetaHat)
{
	double sum = 0;
	int vectSize = Theta->sPrime.size();
	int count = 0;
	double ThetaProb, ThetaHatProb, diff;
	State *curState;
	StateCounter *ThetaHatState = NULL;
	int sPrime;

	for (sPrime = 0; sPrime < vectSize; sPrime++)
	{
		curState = &Theta->sPrime[sPrime].s;
		if (!findStateCounterIndex(&ThetaHat->sPrime, curState, &ThetaHatState))
		{
			/* State in Theta but not ThetaHat: predicted prob is 0 */
			ThetaProb = GetProbability(Theta, sPrime);
			diff = ThetaProb;
			sum += diff * diff;
			count++;
		}
		else
		{
			ThetaProb = GetProbability(Theta, sPrime);
			ThetaHatProb = GetProbability(*ThetaHat, *ThetaHatState);
			diff = ThetaProb - ThetaHatProb;
			sum += diff * diff;
			count++;
		}
	}

	if (count == 0)
		return 0;
	return sum / count;
}

void LP_alg::UpdateStateTransitionVectorTransitions(StateTransitionVector *Theta, StateTransition sas)
{
	StateCounter *tempStateCounter = NULL;

	if (findStateCounterIndex(&Theta->sPrime, &sas.sPrime, &tempStateCounter))
		tempStateCounter->count += 1;
	else
	{
		tempStateCounter = new StateCounter;
		Theta->sPrime.push_back(*tempStateCounter);
		Theta->sPrime.back().count += 1;
		delete tempStateCounter;

		tempStateCounter = &Theta->sPrime.back();
		State ObservedState = {sas.sPrime.StateId, sas.sPrime.Noise};
		copy_State(&ObservedState, &tempStateCounter->s);
	}
	Theta->totalCount += 1;
}

void LP_alg::UpdateTransitions(StateTransition sas)
{
	int s = sas.s.StateId;
	StateCounter *tempStateCounter = NULL;

	if (findStateCounterIndex(&transitionCount[s][sas.a].sPrime, &sas.sPrime, &tempStateCounter))
		tempStateCounter->count += 1;
	else
	{
		tempStateCounter = new StateCounter;
		transitionCount[s][sas.a].sPrime.push_back(*tempStateCounter);
		transitionCount[s][sas.a].sPrime.back().count += 1;
		delete tempStateCounter;

		tempStateCounter = &transitionCount[s][sas.a].sPrime.back();
		State ObservedState = {sas.sPrime.StateId, sas.sPrime.Noise};
		copy_State(&ObservedState, &tempStateCounter->s);
	}
	transitionCount[s][sas.a].totalCount += 1;
}

double LP_alg::GetProbability(StateTransitionVector sas, StateCounter s)
{
	return GetProbability(s.count, sas.totalCount);
}

double LP_alg::GetProbability(StateCounter s, int totalCount)
{
	return GetProbability(s.count, totalCount);
}

double LP_alg::GetProbability(StateTransitionVector *sas, int stateIndex)
{
	return GetProbability(sas->sPrime[stateIndex], sas->totalCount);
}

double LP_alg::GetProbability(int visitCount, int totalCount)
{
	return double(visitCount)/totalCount;
}
