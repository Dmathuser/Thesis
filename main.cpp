#include <iostream>
#include <iomanip>
#include "simulation.h"

using namespace std;

int main(int argc, char** argv)
{
	// Initialize variables
	int numEpochs = 1;
	int numEpisodes = 1;
	int episodeLength = 100;

	// Data Arguments
	// Load Simulation from File
	Simulation sim = Simulation();
	sim.initializeSim("TestSim.sim");
	sim.printSim();
	// Set variables
	// Epochs Loop (Loop once for each Episode)
		// Episode Loop (Loop over time-steps t)
  return 0;
}
