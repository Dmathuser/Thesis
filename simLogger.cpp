#include "simLogger.h"

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

SimLogger::~SimLogger()
{
}

bool SimLogger::startLog()
{
	startTime = std::chrono::high_resolution_clock::now();
	return false;
}

bool SimLogger::log()
{
	//Fill in SimLogs structure
	logs.distractionRate.push_back(sim->getNoiseCount());
	//std::cout << "Successfully Update distractionRate" << std::endl;
	//If an optimal policy has been found, stop timer and log time
	if (isPolicyOptimal())
		stopLog();
	//calculate KL-divergence of current model to true model
	double policyAccuracy = getPolicyAccuracy();
	//std::cout << "Successfully got policyAccuracy: " << policyAccuracy << std::endl;
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
	fout << "Time taken by function: "
         << logs.learningRateCount << " milliseconds" << std::endl;
	fout << "TimeStep, Distraction Rate, Model Accuracy" << std::endl;
	for (int i = 0; i < logs.distractionRate.size(); i++)
	{
		fout << i << ", " 
			<< logs.distractionRate[i] << ", "
			<< logs.modelAccuracy[i] << std::endl;
	}
	return true;
}
bool SimLogger::isPolicyOptimal()
{
	return false;
}

double SimLogger::getPolicyAccuracy()
{
	double MissingInformation = 0;
	int numStates = policy->getNumStates();
	int ***theta = sim->getTransitions();
	int ***thetahat = policy->getTransitionCount();
	//std::cout << "Successfully Initialize data" << std::endl;
	//DOUBLE CHECK FOR SPEED
	for (int s = 0; s < numStates; s++)
	{
		for (int a = 0; a < ACTION_SIZE; a++)
		{
			double KL_div = KL_D(theta[s][a], thetahat[s][a]);
			//double KL_div = KL_D(thetahat[s][a], theta[s][a]); //TEST
			if (!isnan(KL_div) && !isinf(KL_div))
			{
				MissingInformation += KL_div;
				//std::cout << "KL_div: " << KL_div << std::endl;
				//std::cout << "MissingInformation: " << MissingInformation << std::endl;
			}
		}
	}
	return MissingInformation;
}

double SimLogger::KL_D(int* Theta, int* ThetaHat)
{
	int N = policy->getN();
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
	return sum;
}
