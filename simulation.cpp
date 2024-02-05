#include "simulation.h"


Simulation::Simulation()
{
}


Simulation::~Simulation()
{
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
	//states = new state(numStates);
	//states = new int(numStates); //Fix to use dynamic allocation later.

	int index = 0;
	// Read in simulation from file.
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			index = height*j+i;
			//Read in state value to state.
			readTile(index);
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
	int index = 0;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			index = height*j+i;
			std::cout << states[index].stateVal;
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
	int index = 0;
	char cornerChar = '+';
	for (int j = 0; j < height; j++)
	{
		for (int k = 0; k < 3; k++) //Loop through lines of printing
		{
			for (int i = 0; i < width; i++)
			{
				index = height*j+i;
				if (k == 0) //If printing top pad Output.
					std::cout << cornerChar << states[index].up << cornerChar;
				else if (k == 1) //If printing left.
					std::cout << states[index].left << states[index].agent << states[index].right;
				else if (k == 2) //If printing bottom pad Output.
					std::cout << cornerChar << states[index].down << cornerChar;
			}
			std::cout << std::endl;
		}
	}
	return true;
}

bool Simulation::readTile(int index)
{
	int value;
	fin >> value;
	states[index].stateVal = value;
	// Store Walls
	if ((value & 15) != 0) // If tile is wall (Any bits 1-4 true)
		if (((value >> 0) & 1) != 0) // If Top tile is Wall (bit 1 is set)
			states[index].up = '-';
		if (((value >> 1) & 1) != 0) // If Bottom tile is Wall (bit 2 is set)
			states[index].down = '-';
		if (((value >> 2) & 1) != 0) // If Left tile is Wall (bit 3 is set)
			states[index].left = '|';
		if (((value >> 3) & 1) != 0) // If Right tile is Wall (bit 4 is set)
			states[index].right = '|';
	// Store Noisy-TV's Overwrite walls if necessary.
	if ((value >> 4) & 15 != 0) // If tile is Noisy-TV (Any bits 5-8 true)
		if (value >> 4 & 1 != 0) // If Top tile is Noisy-TV (bit 5 is set)
			states[index].up = '^';
		if (value >> 5 & 1 != 0) // If Bottom tile is Noisy-TV (bit 6 is set)
			states[index].down = 'v';
		if (value >> 6 & 1 != 0) // If Left tile is Noisy-TV (bit 7 is set)
			states[index].left = '<';
		if (value >> 7 & 1 != 0) // If Right tile is Noisy-TV (bit 8 is set)
			states[index].right = '>';
	// Store Agent if Valid.
	if ((value >> 8) & 1 != 0)
		states[index].agent = 'X';
	std::cout << value << ", " << index << std::endl;
	std::cout << (value >> 0 & 1) << ", ";
	std::cout << (value >> 1 & 1) << ", ";
	std::cout << (value >> 2 & 1) << ", ";
	std::cout << (value >> 3 & 1) << ", " << std::endl;
	std::cout << states[index].up << ", ";
	std::cout << states[index].down<< ", ";
	std::cout << states[index].left << ", ";
	std::cout << states[index].right << std::endl;
	
	return true;
}
