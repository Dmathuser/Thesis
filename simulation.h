#ifndef _SIMULATION_H
#define _SIMULATION_H
#include <iostream>
#include <fstream>
#include <string>

const char AGENT_CHAR = 'X';
const char NO_AGENT_CHAR = ' ';
const char BAD_INDEX = -1;
const char NO_WALL = ' ';

struct State
{
	char up = NO_WALL;
	char down = NO_WALL;
	char left = NO_WALL;
	char right = NO_WALL;
	char agent = NO_AGENT_CHAR;
	State* upState = nullptr;
	State* downState = nullptr;
	State* leftState = nullptr;
	State* rightState = nullptr;
	//Index stateIndex; //State Index in states array.
	int stateVal = 0; //Temp Variable that stores data in compact form.
};

struct Index
{
	int x = -1; //Width Index
	int y = -1; //Height Index
};

enum Action {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};

class Simulation {
	private:
		int width = 0;
		int height = 0;
		int numStates = 0;
		int noisy = 0;
		State **states; //Dynamic Memory Allocation 
		std::ifstream fin;
		bool readTile(int row, int col);
		bool connectStates();
		bool isValidSim(); // Checks that edges of sim are all walls
	public:
		Simulation();
		Simulation(std::string filename);
		~Simulation();
		bool initializeSim(std::string filename);
		bool printSim();
		bool printSimFancy();
		bool printSimPDF(std::string outFileName);

		//OLD VERSIONS BELOW
		State* getCurState();
		bool setCurState(int stateIndex);
		bool moveState(Action a);
		bool isValidMove(State s, Action a);
		bool isNoisyMove(State s, Action a);
};

#endif
