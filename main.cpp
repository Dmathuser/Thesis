#include <iostream>
#include <iomanip>
#include "simulation.h"

using namespace std;

void TestSim(string filename);

int main(int argc, char** argv)
{
	// Initialize variables
	int numEpochs = 1;
	int numEpisodes = 1;
	int episodeLength = 100;

	// Data Arguments
	// Load Simulation from File
	TestSim("TestSim.sim");
	// Set variables
	// Epochs Loop (Loop once for each Episode)
		// Episode Loop (Loop over time-steps t)
  return 0;
}

void TestSim(string filename)
{
	string outFilename;
	Simulation sim = Simulation();
	sim.initializeSim(filename);
	sim.printSim();
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
