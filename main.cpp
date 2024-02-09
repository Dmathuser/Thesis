#include <iostream>
#include <iomanip>
#include "simulation.h"
#include "randWalk.h"

using namespace std;

void TestSim(string filename);
void TestPolicy(string filename, int seed);

int main(int argc, char** argv)
{
	// Initialize variables
	int numEpochs = 1;
	int numEpisodes = 1;

	// Data Arguments
	// Load Simulation from File
	TestPolicy("TestSim.sim", 42);
	//TestSim("TestSim.sim");
	// Set variables
	// Epochs Loop (Loop once for each Episode)
		// Episode Loop (Loop over time-steps t)
  return 0;
}

void TestPolicy(string filename, int seed)
{
	int episodeLength = 100;
	Simulation sim = Simulation(filename);
	sim.setCurState(5);
	sim.printSimFancy();
	RandWalk policy = RandWalk(seed);
	State s;
	State sPrime;
	Action a;
	StateTransition sas;
	for (int i = 0; i < episodeLength; i++)
	{
		a = policy.getAction(s);
		sim.moveState(a);
		sas.s = s;
		sas.a = a;
		sPrime.StateId = sim.getCurStateIndex(); //Get State ID for new state
		sPrime.Noise = s.Noise; //Propegate Noise forward through states
		sas.sPrime = sPrime; //Set next state for state Transition
		policy.Update(sas);
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
		//Update s in time.
		s = sPrime;
	}
	cout << endl;
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
