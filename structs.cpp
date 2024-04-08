#include "structs.h"
#include <iostream>

const bool isStateEqual(State *s1, State *s2)
{
	//Check for nullptr
	if (s1 != nullptr || s2 != nullptr)
	{
		if(s1->StateId == s2->StateId && s1->Noise == s2->Noise)
			return true;
		else
			return false;
	}
	std::cout << "ERROR: One or more States in comparison are nullptr" << std::endl;
	return false;
}

//State comparison that ignores State noise component.
const bool isNoisyStateEqual(State *s1, State *s2)
{
	if (s1 != nullptr || s2 != nullptr)
	{
		if(s1->StateId == s2->StateId) 
				return true;
		else
			return false;
	}
	std::cout << "ERROR: One or more States in comparison are nullptr" << std::endl;
	return false;
}

//State comparison that ignores State noise component.
const bool isNoisyStateEqual(State *s1, int s2_Id)
{
	if (s1 != nullptr)
	{
		if(s1->StateId == s2_Id)
			return true;
		else
			return false;
	}
	std::cout << "ERROR: One or more States in comparison are nullptr" << std::endl;
	return false;
}

const int getNoisyStateCount(StateTransitionVector *sas, int stateId)
{
	int sum = 0;
	//loop thorugh transitionVector, If stateId matches, add count to total
	for (int i = 0; i < int(sas->sPrime.size()); i++)
		if (isNoisyStateEqual(&sas->sPrime[i].s, stateId))
		{
			//std::cout << "Add to noisySum: " << sas->sPrime[i].count << std::endl;
			sum += sas->sPrime[i].count;
		}
	return sum;
}

//Used to find the location of a known state in a given StateTransitionVector. Returns true if found, false if not. A pointer to the found StateCounter is returned by reference.
bool findStateCounterIndex(std::vector<StateCounter> *sVec, State *s, StateCounter **foundStateCounter)
{
	StateCounter *tmp = nullptr;
	//itterate through vector, return stateCounter if found.
	for (int i = 0; i < int(sVec->size()); i++)
	{
		tmp = &sVec->at(i);
		if (isStateEqual(&tmp->s, s))
		{
			//std::cout << "Successfully found tmp: " << tmp << std::endl;
			*foundStateCounter = tmp;
			return true;
		}
	}
	return false;
}

bool copy_State(State *src, State *dest)
{
	if(dest == nullptr || src == nullptr)
		return false;
	dest->StateId = src->StateId;
	dest->Noise = src->Noise;
	return true;
}
bool copy_StateCounter(StateCounter *src, StateCounter *dest)
{
	if(dest == nullptr || src == nullptr)
		return false;
	if(!copy_State(&src->s, &dest->s))
		return false;
	dest->count = src->count;
	return true;
}

bool copy_StateTransitionVector(StateTransitionVector *src, StateTransitionVector *dest)
{
	if(dest == nullptr || src == nullptr)
		return false;
	if(!copy_State(&src->s, &dest->s))
		return false;
	dest->a = src->a;
	for(int i = 0; i < int(src->sPrime.size()); i++)
	{
		if(i >= int(dest->sPrime.size())) //Make destination bigger if it is too small.
		{
			StateCounter *tmp = new StateCounter;
			dest->sPrime.push_back(*tmp);
			delete tmp;
		}
		if(!copy_StateCounter(&src->sPrime[i], &dest->sPrime[i]))
			return false;
	}
	dest->totalCount = src->totalCount;
	return true;
}
