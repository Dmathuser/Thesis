
#include <simulation.h>

using namespace std;

bool Simulation::printSimPDF()
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
