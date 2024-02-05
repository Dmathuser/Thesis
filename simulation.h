#ifndef _SIMULATION_H
#define _SIMULATION_H
#include <iostream>
#include <fstream>
#include <string>


struct state
{
	char up = ' ';
	char down = ' ';
	char left = ' ';
	char right = ' ';
	char agent = ' ';
	int stateVal = 0; //Temp Variable that stores data in compact form.
};

class Simulation {
	private:
		int width = 0;
		int height = 0;
		int numStates = 0;
		state states[16]; //Change to dynamic states later
		std::ifstream fin;
	public:
		Simulation();
		~Simulation();
		bool initializeSim(std::string filename);
		bool printSim();
		bool printSimFancy();
		bool printSimPDF(std::string outFileName);
		bool readTile(int index);

};

#endif
