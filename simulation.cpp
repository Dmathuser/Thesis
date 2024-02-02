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

	int tmp = 0;
	// Read in simulation from file.
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			//Read in state value to state.
			fin >> states[height*j+i];
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
			std::cout << states[height*j+i];// << "," << height*j+i;
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


