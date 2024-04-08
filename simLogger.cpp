#include "simLogger.h"
#include "structs.h"

SimLogger::SimLogger(Simulation *simulation, Policy *policy_)
{
	if (simulation != nullptr)
		sim = simulation;
	else
		std::cout << "Error: Given Simulation is Null" << std::endl;

	if (policy_ != nullptr)
		policy = policy_;
	else
		std::cout << "Error: Given Policy is Null" << std::endl;
}

SimLogger::SimLogger(Simulation *simulation, Policy *policy_, int simSeed)
{
	logs.seed = simSeed;
	if (simulation != nullptr)
		sim = simulation;
	else
		std::cout << "Error: Given Simulation is Null" << std::endl;

	if (policy_ != nullptr)
		policy = policy_;
	else
		std::cout << "Error: Given Policy is Null" << std::endl;
}

SimLogger::SimLogger(Simulation *simulation, Policy *policy_, std::string SimName, int simSeed)
{
	logs.seed = simSeed;
	SimulationName = SimName;
	if (simulation != nullptr)
		sim = simulation;
	else
		std::cout << "Error: Given Simulation is Null" << std::endl;

	if (policy_ != nullptr)
		policy = policy_;
	else
		std::cout << "Error: Given Policy is Null" << std::endl;
}

SimLogger::~SimLogger()
{
}

bool SimLogger::startLog()
{
	startTime = std::chrono::high_resolution_clock::now();
	return true;
}

bool SimLogger::log()
{
	stopTime = std::chrono::high_resolution_clock::now();
	using FloatMilli = std::chrono::duration<double, std::chrono::milliseconds::period>;
	logs.time.push_back(FloatMilli(stopTime - startTime).count());
	//Fill in SimLogs structure
	logs.distractionRate.push_back(sim->getNoiseCount());
	//std::cout << "Successfully Update distractionRate" << std::endl;
	//If an optimal policy has been found, stop timer and log time
	//if (isPolicyOptimal())
		//std::cout << "Current Time is: " << logs.distractionRate.size();
		//stopLog();
	//calculate KL-divergence of current model to true model
	double policyAccuracy = getPolicyAccuracy();

	int infCount = getPolicyInfCount();
	logs.Infinity_Count.push_back(infCount);

	//logs.modelAccuracy.push_back(getPolicyAccuracy());
	logs.modelAccuracy.push_back(policyAccuracy);
	//std::cout << "Successfully Update modelAccuracy" << std::endl;
	return true;
}

bool SimLogger::stopLog()
{
	stopTime = std::chrono::high_resolution_clock::now();
	using FloatMilli = std::chrono::duration<double, std::chrono::milliseconds::period>;
	//logs.learningRateCount = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count();
	logs.learningRateCount = FloatMilli(stopTime - startTime).count();
	//logs.learningRateCount.count() = duration.count();
	return false;
}

bool SimLogger::printSimLogs(std::string outFileName)
{
	fout.open(outFileName);
	if (!fout.is_open())
	{
		std::cout << "ERROR: Could not open logging file \"" << outFileName << "\"" << std::endl;
		return false;
	}
	fout << "Seed is: " << logs.seed << std::endl;
	fout << "Number of samples is: " << logs.distractionRate.size() << std::endl;
	if (SimulationName != "")
		fout << "Simulation name is: " << SimulationName << std::endl;
	//fout << "Time taken by function: "
    //     << logs.learningRateCount << " milliseconds" << std::endl;
	fout << "TimeStep, Distraction Rate, Missed States, Model Accuracy, Time (ms)" << std::endl;
	for (int i = 0; i < int(logs.distractionRate.size()); i++)
	{
		fout << i << ", " 
			<< logs.distractionRate[i] << ", "
			<< logs.Infinity_Count[i] << ", "
			<< logs.modelAccuracy[i] << ", "
			<< logs.time[i] << std::endl;
	}
	return true;
}
bool SimLogger::isPolicyOptimal()
{
	if (getPolicyInfCount() == 0)
		return true;
	return false;
}

double SimLogger::getPolicyAccuracy()
{
	double MissingInformation = 0;
	int numStates = policy->getNumStates();
	int ***theta = sim->getTransitions();
	StateTransitionVector **thetahat = policy->getTransitionCount();
	//std::cout << "Successfully Initialize data" << std::endl;
	//DOUBLE CHECK FOR SPEED
	//std::cout << "Successfully entered policyAccuracy: " << std::endl;
	//std::cout << "---------------POLICY LOOP START---------------------" << std::endl;
	for (int s = 0; s < numStates; s++)
	{
		for (int a = 0; a < ACTION_SIZE; a++)
		{
			//std::cout << "-------------------------------------------" << std::endl;
			double KL_div = KL_D(theta[s][a], &thetahat[s][a]);
			//if (a == 0)
				//std::cout << "Successfully entered policyAccuracy: " << std::endl;
			//double KL_div = KL_D(thetahat[s][a], theta[s][a]); //TEST
			if (!isnan(KL_div) && !isinf(KL_div))
			{
				//std::cout << "PolicyAccuracy KL_div is: " << KL_div << std::endl;
				MissingInformation += KL_div;
				//std::cout << "KL_div: " << KL_div << std::endl;
				//std::cout << "MissingInformation: " << MissingInformation << std::endl;
			}
		}
	}
	return MissingInformation;
}

int SimLogger::getPolicyInfCount()
{
	int infCount = 0;
	int numStates = policy->getNumStates();
	int ***theta = sim->getTransitions();
	StateTransitionVector **thetahat = policy->getTransitionCount();
	for (int s = 0; s < numStates; s++)
		for (int a = 0; a < ACTION_SIZE; a++)
			infCount += KL_D_InfCount(theta[s][a], &thetahat[s][a]);
	return infCount;
}

int SimLogger::KL_D_InfCount(int* Theta, StateTransitionVector* ThetaHat)
{
	int count = 0;
	int N = sim->getNumStates() + 1;

	//Check for empty transition Vector
	/*
	if (Theta[N-1] == 0 || ThetaHat->totalCount == 0)
	{
		if (Theta[N-1] == 0 && ThetaHat->totalCount == 0)
			std::cout << "Empty transition Vector: (" << Theta[N-1] << "," << ThetaHat->totalCount << ")" << std::endl;
		return 1;
	}
	*/

	// Loop through all possible state transitions given s and a.
	for (int sPrime = 0; sPrime < N-1; sPrime++)
	{
		//Only check state transition if possibility is nonzero
		if (Theta[sPrime] != 0)
		{
			//get count of Thetahat sas' transition ignoring noise
			int ThetaHatCount = getNoisyStateCount(ThetaHat, sPrime);
			//if (ThetaHatCount == 0) //IF ThetaHat does NOT have Transition.
				//count += 1;
			//Transform integer counts into transition probabilites.
			double transitionProb = double(Theta[sPrime]) / Theta[N-1];
			double transitionProbHat = double(ThetaHatCount) / ThetaHat->totalCount;
			//EQUATION: Theta(sas') * Log_2((Theta(sas')/ThetaHat(sas')))
			double temp = transitionProb/transitionProbHat;
			double ans = transitionProb * log2(temp);

			if (isnan(ans) || isnan(temp) || isinf(temp))
			{
				count += 1;
			}
		}
	}
	return count;
}

double SimLogger::KL_D(int* Theta, StateTransitionVector* ThetaHat)
{
	int N = sim->getNumStates() + 1;

	//Check for empty transition Vector
	if (Theta[N-1] == 0 || ThetaHat->totalCount == 0)
	{
		if (Theta[N-1] == 0 && ThetaHat->totalCount == 0)
			std::cout << "Empty transition Vector: (" << Theta[N-1] << "," << ThetaHat->totalCount << ")" << std::endl;
		return std::numeric_limits<double>::infinity();
	}
	double sum = 0;
	int count = 0;

	/*
	std::cout << std::endl << "ThetaHat (" << ThetaHat->s.StateId << ", " << ThetaHat->a << ") Vector of counters: ";
	// For Each vector element, Print (stateId, Noise, Count)
	for (int i = 0; i < ThetaHat->sPrime.size(); i++)
		std::cout << "(" << ThetaHat->sPrime[i].s.StateId << ", " << ThetaHat->sPrime[i].s.Noise << ", " << ThetaHat->sPrime[i].count << ")" << ", ";
	std::cout << std::endl; 
	*/	
	// Loop through all possible state transitions given s and a.
	for (int sPrime = 0; sPrime < N-1; sPrime++)
	{
		//Only check state transition if possibility is nonzero
		if (Theta[sPrime] != 0)
		{
			//get count of Thetahat sas' transition ignoring noise
			//std::cout << "sPrime = " << sPrime << std::endl;
			int ThetaHatCount = getNoisyStateCount(ThetaHat, sPrime);
			if (ThetaHatCount != 0)
				count += 1;
				//return std::numeric_limits<double>::infinity();
			//Transform integer counts into transition probabilites.
			//TODO?, update to use GetProbability function.
			double transitionProb = double(Theta[sPrime]) / Theta[N-1];
			double transitionProbHat = double(ThetaHatCount) / ThetaHat->totalCount;
			//EQUATION: Theta(sas') * Log_2((Theta(sas')/ThetaHat(sas')))
			double temp = transitionProb/transitionProbHat;
			double ans = transitionProb * log2(temp);
			//if (transitionProb == 0)
			/*
			if (ThetaHatCount != 0)
				std::cout << "[Theta,ThetaHat]: [" << transitionProb << ", " << transitionProbHat << "]" << std::endl;
				*/
			if (ans != 0 && !isnan(ans) && !isnan(temp) && !isinf(temp))
			{
				sum += ans;
				//std::cout << "Logging KL_D sum is now: " << sum << std::endl;
			}
		}
	}
	/*
	std::cout << std::endl;
	std::cout << "Theta:    [";
	for (int i = 0; i < N-1; i++)
	{
		//double transitionProb = double(Theta[sPrime]) / Theta[N-1];
		std::cout << Theta[i] << ", ";
	}
	std::cout << Theta[N-1] << "]" << std::endl;
	std::cout << "ThetaHat: [";
	for (int i = 0; i < N-1; i++)
	{
		//double transitionProbHat = double(ThetaHat[sPrime]) / ThetaHat[N-1];
		std::cout << ThetaHat[i] << ", ";
	}
	std::cout << ThetaHat[N-1] << "]" << std::endl;
	if (sum != 0)
		std::cout << "KL_div = " << sum << std::endl;
	*/
	//std::cout << "Count: " << count << std::endl;
	return sum;
}
