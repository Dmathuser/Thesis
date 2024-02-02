#ifndef _SIMULATION_H
#define _SIMULATION_H
#include <iostream>
#include <fstream>
#include <string>

/*
struct state
{
	int val;
};
*/
class Simulation {
	private:
		int width = 0;
		int height = 0;
		int numStates = 0;
		int states[9]; //Change to dynamic states later
		std::ifstream fin;
	public:
		Simulation();
		~Simulation();
		bool initializeSim(std::string filename);
		bool printSim();

};

#endif
