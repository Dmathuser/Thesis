Thesis Project Created by David Mathews

# Goals:
- Create Simulation for Research.
	- Load Simulation from file
	- Data Arguments
		- Flags for picking algorithm type
		- \# of epochs
		- \# of time-steps in each epoch
		- Random Seed
		- File name for Simulation
		- Output File name?
- Code Algorithms (PIG, PIG++, Random Walk)
- Create Datafile.

# TODO:
## Step 1:
- [x] Load Simulation from file
- [] Create Random Walk
- [] Output Data in usable format
- [] Create Python Script to graph Data

## Step 2:
- [] Work on PIG.
- [] Work on PIG++

## Step 3:
- [] Get Data Arguments Working.
- [] Parallel Process Simulations
- [] Create Multiple Simulation Files (For wider testing)

## Step 4:
- [] Data Gathering, Processing, and Graphing

## Simulation Details
### Simulation File Formatting
- First Line: Width and Height separated by a single space
- Data Section: Read in as Integers. Each Integer is a state in the simulation with the following formatting.
  - Bits 1-4: Is there a wall in (Top[1], Bottom[2], Left[3], Right[4]). 1 if yes.
	- Bits 5-8: Is there a Noisy TV Facing inwards in (Top[5], Bottom[6], Left[7], Right[8]) 1 if yes.
	- Bit 9: Is the Agent in this state. 1 if yes.
