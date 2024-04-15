#include <iostream>
#include <iomanip>
#include "simulation.h"
#include "simLogger.h"
#include "randWalk.h"
#include "PIG.h"
#include "EPIG.h"
#include "TPIG.h"
#include "ETPIG.h"

using namespace std;

string SetOutputFileExtension(string fileName, int outNumber, int epsilon);
string SetOutputFileExtension(string filename, int outNumber);
//void RunSimulation(string simFile, string outFile, int seed);
void RunPIGSimulation(string simFile, string outFile, int episodeLength, int seed);
void RunEPIGSimulation(string simFile, string outFile, int episodeLength, int epsilon, int seed);
void RunRandSimulation(string simFile, string outFile, int episodeLength, int seed);
void RunETPIGSimulation(string simFile, string outFile, int episodeLength, int epsilon, int seed);
void RunTPIGSimulation(string simFile, string outFile, int episodeLength, int seed);
void TestSim(string filename);
void TestPolicy(string filename, int seed);
void printAction(Action a);
StateTransition getSAS(State s, Action a, Simulation *sim);

int main(int argc, char** argv)
{
	// Initialize variables
	int numEpochs = 1;
	int numEpisodes = 100;
	string simFile = "Simulation.sim";
	//string simFile = "TestSim.sim";
	//string fileName = "SimulationData/EPIG_1.txt";
	string TPIGfileName = "SimulationData/TPIG_1.txt";
	string ETPIGfileName = "EpsilonData/ETPIG_1.txt";
	string EPIGfileName = "EpsilonData/EPIG_1.txt";
	string PIGfileName = "SimulationData/PIG_1.txt";
	string RandfileName = "SimulationData/Rand_1.txt";
	string outFile;
	int episodeLength = 1000;
	int seed = 0;
	int count = 0;
	
	for (int i = 0; i < numEpochs; i++)
		for (int j = 0; j < numEpisodes; j++)
		{
			cout << "Starting Trial " << count+1 << " of " << numEpochs*numEpisodes << endl;
			seed = count;
			/*
			cout << "    Part 1: TPIG" << endl;
			outFile = SetOutputFileExtension(TPIGfileName, count);
			RunTPIGSimulation(simFile, outFile, episodeLength, seed);
			*/

			for (int epsilon = 0; epsilon <= 100; epsilon += 10)
			{
				cout << "  Epsilon = " << epsilon << endl;
				cout << "    Part 2: ETPIG" << endl;
				outFile = SetOutputFileExtension(ETPIGfileName, count, epsilon);
				RunETPIGSimulation(simFile, outFile, episodeLength, epsilon, seed);

				cout << "    Part 3: EPIG" << endl;
				outFile = SetOutputFileExtension(EPIGfileName, count, epsilon);
				RunEPIGSimulation(simFile, outFile, episodeLength, epsilon, seed);
			}

			/*	
			cout << "    Part 4: PIG" << endl;
			outFile = SetOutputFileExtension(PIGfileName, count);
			RunPIGSimulation(simFile, outFile, episodeLength, seed);

			cout << "    Part 5: RandWalk" << endl;
			outFile = SetOutputFileExtension(RandfileName, count);
			RunRandSimulation(simFile, outFile, episodeLength, seed);
			*/

			count += 1;
		}

	// Data Arguments
	// Load Simulation from File
	//TestPolicy("TestSim.sim", 42);
	//TestPolicy("NoTvSim.sim", 42);
	//TestPolicy("Simulation.sim", 42);
	//TestSim("TestSim.sim");
	// Set variables
	// Epochs Loop (Loop once for each Episode)
		// Episode Loop (Loop over time-steps t)
  return 0;
}
string SetOutputFileExtension(string fileName, int outNumber, int epsilon)
{
	string outFileName;
	if(fileName.find_last_of('.') != string::npos)
	{
		string extension = fileName.substr(fileName.find_last_of('.'),4);

    outFileName = fileName.substr(0,fileName.find_last_of('.'));
		outFileName = outFileName + "_e" + to_string(epsilon);
		outFileName = outFileName + "_" + to_string(outNumber) + extension;
	}
	else
		outFileName = fileName;
	return outFileName;
}

string SetOutputFileExtension(string fileName, int outNumber)
{
	string outFileName;
	if(fileName.find_last_of('.') != string::npos)
	{
		string extension = fileName.substr(fileName.find_last_of('.'),4);

    outFileName = fileName.substr(0,fileName.find_last_of('.'));
		outFileName = outFileName + "_" + to_string(outNumber) + extension;
	}
	else
		outFileName = fileName;
	return outFileName;
}

void RunETPIGSimulation(string simFile, string outFile, int episodeLength, int epsilon, int seed)
{
	Simulation sim = Simulation(simFile, seed);
	//Setup Logger
	ETPIG_alg policy = ETPIG_alg(seed,epsilon,sim.getNumStates(),sim.getNumActions());
	SimLogger logger = SimLogger(&sim, &policy, simFile, sim.getSeed());

	sim.setCurState(5); //Set starting state for agent in simulation
	State s = {5};
	Action a = UP;
	StateTransition sas = {0};

	logger.startLog();
	for (int i = 0; i < episodeLength; i++)
	{
		logger.log();
		//std::cout << std::endl << "------- Main Get Action ------- " << std::endl;
		a = policy.getAction(s);

		sim.moveState(a);
		sas = getSAS(s, a, &sim); //Data Leak? Look up returning structs.

		//std::cout << "--------- Main Update --------- " << std::endl;
		policy.Update(sas);
		s = sas.sPrime;
	}
	logger.stopLog();
	logger.printSimLogs(outFile);
}

void RunTPIGSimulation(string simFile, string outFile, int episodeLength, int seed)
{
	Simulation sim = Simulation(simFile, seed);
	//Setup Logger
	TPIG_alg policy = TPIG_alg(seed,sim.getNumStates(),sim.getNumActions());
	SimLogger logger = SimLogger(&sim, &policy, simFile, sim.getSeed());

	sim.setCurState(5); //Set starting state for agent in simulation
	State s = {5};
	Action a = UP;
	StateTransition sas = {0};

	logger.startLog();
	for (int i = 0; i < episodeLength; i++)
	{
		logger.log();
		//std::cout << std::endl << "------- Main Get Action ------- " << std::endl;
		a = policy.getAction(s);

		sim.moveState(a);
		sas = getSAS(s, a, &sim); //Data Leak? Look up returning structs.

		//std::cout << "--------- Main Update --------- " << std::endl;
		policy.Update(sas);
		s = sas.sPrime;
	}
	logger.stopLog();
	logger.printSimLogs(outFile);
}

void RunEPIGSimulation(string simFile, string outFile, int episodeLength, int epsilon, int seed)
{
	Simulation sim = Simulation(simFile, seed);
	//Setup Logger
	EPIG_alg policy = EPIG_alg(seed, epsilon, sim.getNumStates(),sim.getNumActions());
	SimLogger logger = SimLogger(&sim, &policy, simFile, sim.getSeed());

	sim.setCurState(5); //Set starting state for agent in simulation
	State s = {5};
	Action a = UP;
	StateTransition sas = {0};

	logger.startLog();
	for (int i = 0; i < episodeLength; i++)
	{
		logger.log();
		a = policy.getAction(s);

		sim.moveState(a);
		sas = getSAS(s, a, &sim); //Data Leak? Look up returning structs.

		policy.Update(sas);
		s = sas.sPrime;
	}
	logger.stopLog();
	logger.printSimLogs(outFile);
}

void RunRandSimulation(string simFile, string outFile, int episodeLength, int seed)
{
	Simulation sim = Simulation(simFile, seed);
	//Setup Logger
	RandWalk policy = RandWalk(seed, sim.getNumStates(), sim.getNumActions());
	SimLogger logger = SimLogger(&sim, &policy, simFile, sim.getSeed());

	sim.setCurState(5); //Set starting state for agent in simulation
	State s = {5};
	Action a = UP;
	StateTransition sas = {0};

	logger.startLog();
	for (int i = 0; i < episodeLength; i++)
	{
		logger.log();
		a = policy.getAction(s);

		sim.moveState(a);
		sas = getSAS(s, a, &sim); //Data Leak? Look up returning structs.

		policy.Update(sas);
		s = sas.sPrime;
	}
	logger.stopLog();
	logger.printSimLogs(outFile);
}

void RunPIGSimulation(string simFile, string outFile, int episodeLength, int seed)
{
	Simulation sim = Simulation(simFile, seed);
	//Setup Logger
	PIG_alg policy = PIG_alg(seed,sim.getNumStates(),sim.getNumActions());
	SimLogger logger = SimLogger(&sim, &policy, simFile, sim.getSeed());

	sim.setCurState(5); //Set starting state for agent in simulation
	State s = {5};
	Action a = UP;
	StateTransition sas = {0};

	logger.startLog();
	for (int i = 0; i < episodeLength; i++)
	{
		logger.log();
		a = policy.getAction(s);

		sim.moveState(a);
		sas = getSAS(s, a, &sim); //Data Leak? Look up returning structs.

		policy.Update(sas);
		s = sas.sPrime;
	}
	logger.stopLog();
	logger.printSimLogs(outFile);
}

void TestPolicy(string filename, int seed)
{
	const string MODE = "OTHER";
	int episodeLength = 200;
	//int epsilon = 10;
	cout << "Start" << endl;
	Simulation sim = Simulation(filename, seed);
	sim.setCurState(5);
	sim.printSimFancy();
	//Setup Logger
	RandWalk policy = RandWalk(seed, sim.getNumStates(), sim.getNumActions());
	//PIG_alg policy = PIG_alg(seed,sim.getNumStates(),sim.getNumActions());
	//EPIG_alg policy = EPIG_alg(seed, epsilon, sim.getNumStates(),sim.getNumActions());
	//TPIG_alg policy = TPIG_alg(seed,sim.getNumStates(),sim.getNumActions());
	SimLogger logger = SimLogger(&sim, &policy, sim.getSeed());

	sim.setCurState(5); //Set starting state for agent in simulation
	State s = {5};
	Action a = UP;
	StateTransition sas = {0};
	cout << "Successfully Initialized" << endl;
	logger.startLog();
	cout << "Successfully Started Logging" << endl;
	cout << "Running Simulation with " << episodeLength << " Timesteps" << endl;
	for (int i = 0; i < episodeLength; i++)
	{
		//std::cout << std::endl << "Pre1 Update - ";
		//policy.PrintTransitionVectors(s, a);

		logger.log();
		a = policy.getAction(s);

		//printAction(a);

		sim.moveState(a);
		sas = getSAS(s, a, &sim); //Data Leak? Look up returning structs.

		policy.Update(sas);
		//logger.log();

		//printAction(a);
		//Take timestep, update s.
		s = sas.sPrime;
	}
	cout << endl;
	logger.stopLog();
	//logger.printSimLogs("Test.txt");
	//logger.printSimLogs("LogsNoTVPIG0.txt");
	if(MODE == "TPIG")
		logger.printSimLogs("LogsTPIG.txt");
	else if (MODE == "EPIG")
		logger.printSimLogs("LogsEPIG0.txt");
	else if (MODE == "PIG")
		logger.printSimLogs("LogsNoTVPig1.txt");
	else if (MODE == "RAND")
		logger.printSimLogs("LogsRandWalk0.txt");
	else
		logger.printSimLogs("LogsNoTVRand1.txt");
}

StateTransition getSAS(State s, Action a, Simulation *sim)
{
  StateTransition sas = {0};
	sas.s = s;
	sas.a = a;
	sas.sPrime.StateId = sim->getCurStateIndex(); //Get State ID for new state
	if (sim->isNoisyMove(s,a))
	{
		sas.sPrime.Noise = sim->getNoiseCount(); //Propegate Noise forward through states
		//cout << "(s,a,s',s'Noise) = (" << s.StateId;
		//printAction(a);
		//cout << ", " << sas.sPrime.StateId << ", " << sas.sPrime.Noise << ")" << endl;
	}
	else
		sas.sPrime.Noise = 0;
	
	//cout << "(s,a,s',s'Noise) = (" << s.StateId;
	//printAction(a);
	//cout << ", " << sas.sPrime.StateId << ", " << sas.sPrime.Noise << ")" << endl;
	return sas;
}

void printAction(Action a)
{
	switch (a)
	{
		case UP:
			cout << ", UP";
			break;
		case DOWN:
			cout << ", DOWN";
			break;
		case LEFT:
			cout << ", LEFT";
			break;
		case RIGHT:
			cout << ", RIGHT";
			break;
		default:
			cout << ", BAD: " << a;
	}
}

void TestSim(string filename)
{
	string outFilename;
	Simulation sim = Simulation();
	sim.initializeSim(filename);
	sim.printSim();
	sim.printSimFancyConnections();

	sim.setCurState(0);
	sim.printSimFancy();

	cout << "Move UP" << endl;
	sim.moveState(UP);
	sim.printSimFancy();
	
	cout << "Move DOWN" << endl;
	sim.moveState(DOWN);
	sim.printSimFancy();
	
	cout << "Move LEFT" << endl;
	sim.moveState(LEFT);
	sim.printSimFancy();
	
	cout << "Move RIGHT" << endl;
	sim.moveState(RIGHT);
	sim.printSimFancy();

	cout << "Move LEFT" << endl;
	sim.moveState(LEFT);
	sim.printSimFancy();

	cout << "Move UP" << endl;
	sim.moveState(UP);
	sim.printSimFancy();
	
	// Took this from printSimPDF.cpp. by Dr. Larry Pyeatt.
	// Should remove .sim extension for output file name purposes.
  if((filename.find_last_of('.') != string::npos) &&
     (filename.substr(filename.find_last_of('.'),4) == ".sim"))
    outFilename=filename.substr(0,filename.find_last_of('.'));
	else
		outFilename = filename;
	sim.printSimPDF(outFilename); 
}
