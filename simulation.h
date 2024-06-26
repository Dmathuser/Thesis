#ifndef _SIMULATION_H
#define _SIMULATION_H
#include <iostream>
#include <fstream>
#include <string>
#include "enums.h"
#include "structs.h"

const char AGENT_CHAR = 'X';
const char NO_AGENT_CHAR = ' ';
const char BAD_INDEX = -1;
const char NO_WALL = ' ';

struct SimState //Used in simulation
{
	char up = NO_WALL;
	char down = NO_WALL;
	char left = NO_WALL;
	char right = NO_WALL;
	char agent = NO_AGENT_CHAR;
	SimState* upState = nullptr;
	SimState* downState = nullptr;
	SimState* leftState = nullptr;
	SimState* rightState = nullptr;
	//Index stateIndex; //State Index in states array.
	int stateIndex = -1;
	int stateVal = 0; //Temp Variable that stores data in compact form.
};

class Simulation {
	private:
		int width = 0;
		int height = 0;
		int numStates = 0;
   	int numActions = 4; // ok, just hardcode it
		int noisy = 0;
		int seed = 0;
		SimState **states; //Dynamic Memory Allocation 
		int ***transitions; //Dynamic Memory Allocation 
		std::ifstream fin;
		bool readTile(int row, int col);
		bool connectStates();
		bool isValidSim(); // Checks that edges of sim are all walls
		bool createTransitionProbs();
	public:
		Simulation();
		Simulation(int seed);
		Simulation(std::string filename);
		Simulation(std::string filename, int seed);
		~Simulation();
		bool initializeSim(std::string filename);
		bool printSim();
		bool printSimFancy();
		bool printSimFancyConnections();
		bool printSimPDF(std::string outFileName);

		SimState* getCurState();
		int getCurStateIndex();
  	int getNumStates(){return numStates;}
  	int getNumActions(){return numActions;}
  	int getNoiseCount(){return noisy;}
  	int getSeed(){return seed;}
  	int*** getTransitions(){return transitions;}
		bool setCurState(int stateIndex);
		bool moveState(Action a);
		bool isValidMove(SimState s, Action a);
		bool isNoisyMove(SimState s, Action a);
		bool isNoisyMove(State s, Action a);
};

#endif
