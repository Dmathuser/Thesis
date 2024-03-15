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
	//If an optimal policy has been found, stop timer and log time
	if (isPolicyOptimal())
		stopLog();
	//calculate KL-divergence of current model to true model
	logs.modelAccuracy.push_back(getPolicyAccuracy());
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
	//DOUBLE CHECK FOR SPEED
	for (int s = 0; s < numStates; s++)
	{
		for (int a = 0; a < ACTION_SIZE; a++)
		{
			MissingInformation += KL_D(theta[a][s], thetahat[a][s]);
		}
	}
	return 0;
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
	return sum;
}
