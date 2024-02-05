#include "simulation.h"


Simulation::Simulation()
{
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
	states = new state*[height];
	for(int i = 0;i < height; i++)
	  states[i] = new state[width];

	// Read in simulation from file.
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
		        //Read in state value to state.
		        readTile(j,i);
			//fin >> states[height*j+i];
			//std::cout << states[height*j+i] << "," << height*j+i << ", ";
		}
		//std::cout << std::endl;
	}
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
	std::cout << width << " " << height << std::endl;
	char cornerChar = '+';
	for (int j = 0; j < height; j++)
	{
		for (int k = 0; k < 3; k++) //Loop through lines of printing
		{
			for (int i = 0; i < width; i++)
			{
				if (k == 0) //If printing top pad Output.
					std::cout << cornerChar << states[j][i].up << cornerChar;
				else if (k == 1) //If printing left.
					std::cout << states[j][i].left << states[j][i].agent << states[j][i].right;
				else if (k == 2) //If printing bottom pad Output.
					std::cout << cornerChar << states[j][i].down << cornerChar;
			}
			std::cout << std::endl;
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
  std::cout << value << ", " << row << "," << col << std::endl;
  std::cout << (value >> 0 & 1) << ", ";
  std::cout << (value >> 1 & 1) << ", ";
  std::cout << (value >> 2 & 1) << ", ";
  std::cout << (value >> 3 & 1) << ", " << std::endl;
  std::cout << states[row][col].up << ", ";
  std::cout << states[row][col].down<< ", ";
  std::cout << states[row][col].left << ", ";
  std::cout << states[row][col].right << std::endl;
	
  return true;
}
