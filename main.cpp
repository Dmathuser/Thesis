#include <iostream>
#include <iomanip>
#include "simulation.h"
#include "simLogger.h"
#include "randWalk.h"
#include "PIG.h"

using namespace std;

void TestSim(string filename);
void TestPolicy(string filename, int seed);
void printAction(Action a);
StateTransition getSAS(State s, Action a, Simulation *sim);

int main(int argc, char** argv)
{
	// Initialize variables
	//int numEpochs = 1;
	//int numEpisodes = 1;

	// Data Arguments
	// Load Simulation from File
	TestPolicy("TestSim.sim", 42);
	//TestPolicy("Simulation.sim", 42);
	//TestSim("TestSim.sim");
	// Set variables
	// Epochs Loop (Loop once for each Episode)
		// Episode Loop (Loop over time-steps t)
  return 0;
}

void TestPolicy(string filename, int seed)
{
	const string MODE = "RAND";
	int episodeLength = 10000;
	cout << "Start" << endl;
	Simulation sim = Simulation(filename);
	sim.setCurState(5);
	sim.printSimFancy();
	//Setup Logger
	//RandWalk policy = RandWalk(seed,sim.getNumStates(),sim.getNumActions());
	/*
	if(MODE == "PIG++")
		Policy policy = nullptr;
	else if (MODE == "PIG")
		PIG_alg policy = PIG_alg(seed,sim.getNumStates(),sim.getNumActions());
	else
		RandWalk policy = RandWalk(seed, sim.getNumStates(), sim.getNumActions());
		*/
	RandWalk policy = RandWalk(seed, sim.getNumStates(), sim.getNumActions());
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

		a = policy.getAction(s);

		//printAction(a);

		sim.moveState(a);
		sas = getSAS(s, a, &sim); //Data Leak? Look up returning structs.

		policy.Update(sas);
		logger.log();

		//printAction(a);
		//Take timestep, update s.
		s = sas.sPrime;
	}
	cout << endl;
	logger.stopLog();
	if(MODE == "PIG++")
		logger.printSimLogs("LogsPigi++.txt");
	else if (MODE == "PIG")
		logger.printSimLogs("LogsPig.txt");
	else
		logger.printSimLogs("LogsRandWalk.txt");
}

StateTransition getSAS(State s, Action a, Simulation *sim)
{
  StateTransition sas = {0};
	sas.s = s;
	sas.a = a;
	sas.sPrime.StateId = sim->getCurStateIndex(); //Get State ID for new state
	sas.sPrime.Noise = s.Noise; //Propegate Noise forward through states
	//cout << "(s,a,s',s'Noise) = (" << s.StateId << ", " << a << ", " << sas.sPrime.StateId << ", " << sas.sPrime.Noise << ")" << endl;
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
