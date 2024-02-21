#include "simulation.h"
#include <iomanip>
#include <stdlib.h>


Simulation::Simulation()
{
}

Simulation::Simulation(int seed)
{
	seed = seed;
	srand(seed); //Might be slow...
}

Simulation::Simulation(std::string filename)
{
	initializeSim(filename);
}

Simulation::~Simulation()
{
  for(int i = 0;i < height; i++)
    delete[] states[i];
  delete[] states;  
}

bool Simulation::initializeSim(std::string filename)
{
	// Open File, Terminate if not open.
	fin.open(filename);
	if (!fin.is_open())
	{
		std::cout << "ERROR: Could not open simulation file \"" << filename << "\"" << std::endl;
		return false;
	}
	// Read in width and height of simulation from file
	fin >> width;
	fin >> height;
	numStates = width * height;

	//Sanity Check
	if (width <= 0 || height <= 0)
	{
		std::cout << "ERROR: Simulation dimensions too small" << std::endl;
		return false;
	}

	// Dynamically allocate memory for simulation state.
	states = new SimState*[height];
	for(int i = 0;i < height; i++)
	  states[i] = new SimState[width];

	// Read in simulation from file.
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
		        //Read in state value to state.
		        readTile(j,i);
						states[j][i].stateIndex = height*j+i;
						//states[j][i].stateIndex.x = i;
						//states[j][i].stateIndex.y = j;
			//fin >> states[height*j+i];
			//std::cout << states[height*j+i] << "," << height*j+i << ", ";
		}
		//std::cout << std::endl;
	}
	fin.close();
	if(connectStates())
		return true;

	std::cout << "ERROR: Unable to connect State Graph" << std::endl;
	return false;
}

bool Simulation::isValidSim()
{
	// Check if upper and lower edges of sim have walls
	for (int j = 0; j < height; j++)
	{
		if (isValidMove(states[j][0], LEFT))
			return false;
		if (isValidMove(states[j][width-1], RIGHT))
			return false;
	}
	// Check if left and right edges of sim have walls
	for (int i = 0; i < width; i++)
	{
		if (isValidMove(states[0][i], UP))
			return false;
		if (isValidMove(states[height-1][i], DOWN))
			return false;
	}
	return true;
}

bool Simulation::connectStates()
{
	SimState *nextState;
	SimState *curState;
	//Check edges of simulation to guarantee walled edges.
	if (!isValidSim())
	{
		std::cout << "ERROR: Simulation perimeter is missing walls." << std::endl;
		return false;
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			curState = &states[j][i];
			//if there is a wall, loop destination back to self
			// Link Upper edge of state
			if (!isValidMove(*curState, UP))
				nextState = curState;
			else //built in error checking, should never link an outer edge outwards
				nextState = &states[j-1][i];
			curState->upState = nextState;
			
			// Link Lower edge of state
			if (!isValidMove(*curState, DOWN))
				nextState = curState;
			else //built in error checking, should never link an outer edge outwards
				nextState = &states[j+1][i];
			curState->downState = nextState;
		
			// Link Left edge of state
			if (!isValidMove(*curState, LEFT))
				nextState = curState;
			else //built in error checking, should never link an outer edge outwards
				nextState = &states[j][i-1];
			curState->leftState = nextState;
	
			// Link Right edge of state
			if (!isValidMove(*curState, RIGHT))
				nextState = curState;
			else //built in error checking, should never link an outer edge outwards
				nextState = &states[j][i+1];
			curState->rightState = nextState;
			//std::cout << "Center, Up, Down, Left, Right: " << curState->stateIndex;
			//std::cout << ", " << curState->upState->stateIndex;
			//std::cout << ", " << curState->downState->stateIndex;
			//std::cout << ", " << curState->leftState->stateIndex;
			//std::cout << ", " << curState->rightState->stateIndex << std::endl;
		}
	}
	//printSimFancyConnections();
	return true;
}

bool Simulation::printSim()
{
	std::cout << width << " " << height << std::endl;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			std::cout << states[j][i].stateVal;
			if (i < width-1)
			{
				std::cout << " ";
				//std::cout << ", ";
			}
		}
		std::cout << std::endl;
	}
	return true;
}

bool Simulation::printSimFancy()
{
	const bool PRINT_STATE_INDEX = false;
	std::cout << width << " " << height << std::endl;
	char cornerChar = '+';
	for (int j = 0; j < height; j++)
	{
		for (int k = 0; k < 3; k++) //Loop through lines of printing
		{
			for (int i = 0; i < width; i++)
			{
				if (k == 0) //If printing top pad Output.
				{
					std::cout << cornerChar << states[j][i].up << cornerChar;
				}
				else if (k == 1) //If printing middle.
				{	
					std::cout << states[j][i].left; 

					if (states[j][i].agent == AGENT_CHAR)
					{
						std::cout << states[j][i].agent; 
					}
					else if (PRINT_STATE_INDEX == true)
					{
						std::cout << states[j][i].stateIndex;
					}
					else 
					{
						std::cout << states[j][i].agent; 
					}

					std::cout << states[j][i].right;
				}
				else if (k == 2) //If printing bottom pad Output.
				{
					std::cout << cornerChar << states[j][i].down << cornerChar;
				}
			}
			std::cout << std::endl;
		}
	}
	return true;
}

bool Simulation::printSimFancyConnections()
{
	std::cout << width << " " << height << std::endl;
	const char cornerChar = '+';
	const int PLOT_WIDTH = 4;
	SimState *curState;
	for (int j = 0; j < height; j++)
	{
		for (int k = 0; k < 3; k++) //Loop through lines of printing
		{
			std::cout << "k: " << k << "  ";
			for (int i = 0; i < width; i++)
			{
				//std::cout << "j: " << j << ", i: " << i << std::endl;
				curState = &states[j][i];
				//std::cout << "curState: " << curState << std::endl;
				//std::cout << "Up, Down, Left, Right: " << curState->upState << ", "; 
				//std::cout << curState->downState << ", ";
				//std::cout << curState->leftState << ", ";
				//std::cout << curState->rightState << ", " << std::endl;
				std::cout << std::setw(PLOT_WIDTH);
				if (k == 0) //If printing top pad Output.
				{
					if (curState->up != ' ') // If upward is a wall
						std::cout << cornerChar << std::setw(PLOT_WIDTH) << 'n' << std::setw(PLOT_WIDTH) << cornerChar;
					else
						std::cout << cornerChar << std::setw(PLOT_WIDTH) << curState->upState->stateIndex << std::setw(PLOT_WIDTH) << cornerChar;
				}
				else if (k == 1) //If printing left.
				{
					//std::cout << "Test";
					//std::cout << curState.leftState;
					if (curState->left != ' ') // If left is a wall
						std::cout << '[';
					else
						std::cout << curState->leftState->stateIndex;
					std::cout << std::setw(PLOT_WIDTH);
					if (curState->agent != ' ') // If center is agent
						std::cout << curState->stateIndex;
					else
						std::cout << curState->stateIndex;
					std::cout << std::setw(PLOT_WIDTH);
					if (curState->right != ' ') // If right is a wall
						std::cout << ']';
					else
						std::cout << curState->rightState->stateIndex;
				}
				else if (k == 2) //If printing bottom pad Output.
				{
					if (curState->down != ' ') // If upward is a wall
						std::cout << cornerChar << std::setw(PLOT_WIDTH) << 'u' << std::setw(PLOT_WIDTH) << cornerChar;
					else
						std::cout << cornerChar << std::setw(PLOT_WIDTH) << curState->rightState->stateIndex << std::setw(PLOT_WIDTH) << cornerChar;
				}
			}
			//std::cout << "Test";
			std::cout << std::endl << std::endl;
			//std::cout << "Test";
		}
	}
	return true;
}

bool Simulation::readTile(int row, int col)
{
  int value;
  fin >> value;
  states[row][col].stateVal = value;
  // Store Walls
  if ((value & 0xF) != 0) // If tile is wall (Any bits 1-4 true)
    if (((value >> 0) & 1) != 0) // If Top tile is Wall (bit 1 is set)
      states[row][col].up = '-';
  if (((value >> 1) & 1) != 0) // If Bottom tile is Wall (bit 2 is set)
    states[row][col].down = '-';
  if (((value >> 2) & 1) != 0) // If Left tile is Wall (bit 3 is set)
    states[row][col].left = '|';
  if (((value >> 3) & 1) != 0) // If Right tile is Wall (bit 4 is set)
    states[row][col].right = '|';
  // Store Noisy-TV's Overwrite walls if necessary.
  if (((value >> 4) & 15) != 0) // If tile is Noisy-TV (Any bits 5-8 true)
    if (((value >> 4) & 1) != 0) // If Top tile is Noisy-TV (bit 5 is set)
      states[row][col].up = '^';
  if (((value >> 5) & 1) != 0) // If Bottom tile is Noisy-TV (bit 6 is set)
    states[row][col].down = 'v';
  if (((value >> 6) & 1) != 0) // If Left tile is Noisy-TV (bit 7 is set)
    states[row][col].left = '<';
  if (((value >> 7) & 1) != 0) // If Right tile is Noisy-TV (bit 8 is set)
    states[row][col].right = '>';
  // Store Agent if Valid.
  if (((value >> 8) & 1) != 0)
    states[row][col].agent = 'X';
	/*
  std::cout << value << ", " << row << "," << col << std::endl;
  std::cout << (value >> 0 & 1) << ", ";
  std::cout << (value >> 1 & 1) << ", ";
  std::cout << (value >> 2 & 1) << ", ";
  std::cout << (value >> 3 & 1) << ", " << std::endl;
  std::cout << states[row][col].up << ", ";
  std::cout << states[row][col].down<< ", ";
  std::cout << states[row][col].left << ", ";
  std::cout << states[row][col].right << std::endl;
	*/
  return true;
}

// Returns pointer to first state with agent.
// If no state with agent is found, returns nullptr.
SimState* Simulation::getCurState()
{
	// Loop through states, return first state with valid agent.  
  for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			if (states[j][i].agent == AGENT_CHAR)
				return states[j]+i;
	return nullptr;
}

int Simulation::getCurStateIndex()
{
	// Loop through states, return first state with valid agent.  
  for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			if (states[j][i].agent == AGENT_CHAR)
			  {
			    // std::cout << " getCurStateIndex: "<<j*width+i<<std::endl;
				return j*width+i;
			  }
	return -1;
}

bool Simulation::setCurState(int stateIndex)
{
	// Find curent state if valid.
	SimState* curState = getCurState();
	// Remove curent agent from state.
	if (curState != nullptr)
		curState->agent = NO_AGENT_CHAR;

	// Check for valid input state.
	if (stateIndex < 0 || stateIndex >= numStates)
		return false;

	// Add agent to given state.
	int column = stateIndex % width;
	int row = stateIndex / width;
	states[row][column].agent = AGENT_CHAR;
	return true;
}

bool Simulation::moveState(Action a)
{
	// Find Current State if valid
	SimState* curState = getCurState();
	//Return if invalid state
	if (curState == nullptr)
		return false;
	
	// Check if move hits Noisy-TV
	if (isNoisyMove(*curState, a))
		noisy += 1; //SET TO RANDOM VARIABLE LATER.
	
	
	//std::cout << "State ID before move: " << curState->stateIndex << std::endl;
	// Find Resulting New State, Move to State
	switch (a)
	{
		case UP:
			curState->agent = NO_AGENT_CHAR;
			if (rand()%100 < 90)
				curState = curState->upState;
			break;
		case DOWN:
			curState->agent = NO_AGENT_CHAR;
			if (rand()%100 < 90)
				curState = curState->downState;
			break;
		case LEFT:
			curState->agent = NO_AGENT_CHAR;
			if (rand()%100 < 90)
				curState = curState->leftState;
			break;
		case RIGHT:
			curState->agent = NO_AGENT_CHAR;
			if (rand()%100 < 90)
				curState = curState->rightState;
			break;
		default:
			return false;
	}
	//std::cout << "State ID after move: " << curState->stateIndex << std::endl;
	curState->agent = AGENT_CHAR;
	//std::cout << "Successfully took move: " << a << std::endl;

	return true; 
}

// Determines if given state s allows for movement in direction a.
bool Simulation::isValidMove(SimState s, Action a)
{
	switch (a)
	{
		case UP:
			if (s.up == NO_WALL)
				return true;
			break;
		case DOWN:
			if (s.down == NO_WALL)
				return true;
			break;
		case LEFT:
			if (s.left == NO_WALL)
				return true;
			break;
		case RIGHT:
			if (s.right == NO_WALL)
				return true;
			break;
		default:
			return false;
	}
	return false;
}

bool Simulation::isNoisyMove(SimState s, Action a)
{
	switch (a)
	{
		case UP:
			if ((((s.stateVal >> 5) & 1) != 0))
				return true;
			break;
		case DOWN:
			if ((((s.stateVal >> 6) & 1) != 0))
				return true;
			break;
		case LEFT:
			if ((((s.stateVal >> 7) & 1) != 0))
				return true;
			break;
		case RIGHT:
			if ((((s.stateVal >> 8) & 1) != 0))
				return true;
			break;
		default:
			return false;
	}
	return false;
}
