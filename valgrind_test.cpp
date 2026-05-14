#include <iostream>
#include "simulation.h"
#include "simLogger.h"
#include "randWalk.h"
#include "PIG.h"
#include "EPIG.h"
#include "TPIG.h"
#include "ETPIG.h"
#include "LP.h"

using namespace std;

StateTransition getSAS(State s, Action a, Simulation *sim)
{
    StateTransition sas = {0};
    sas.s = s;
    sas.a = a;
    sas.sPrime.StateId = sim->getCurStateIndex();
    if (sim->isNoisyMove(s, a))
        sas.sPrime.Noise = sim->getNoiseCount();
    else
        sas.sPrime.Noise = 0;
    return sas;
}

void runPolicy(const char *name, Policy *pol, Simulation *sim, int steps)
{
    sim->setCurState(5);
    State s = {5, 0};
    Action a = UP;
    StateTransition sas = {0};
    int i;

    for (i = 0; i < steps; i++) {
        a = pol->getAction(s);
        sim->moveState(a);
        sas = getSAS(s, a, sim);
        pol->Update(sas);
        s = sas.sPrime;
    }
    cout << "  " << name << ": OK" << endl;
}

int main()
{
    int nS, nA, steps;

    steps = 100;

    cout << "Testing on Simulation_1TV.sim..." << endl;
    {
        Simulation sim("Simulation_1TV.sim", 42);
        nS = sim.getNumStates();
        nA = sim.getNumActions();

        RandWalk rw(42, nS, nA);
        runPolicy("RandWalk", &rw, &sim, steps);
    }
    {
        Simulation sim("Simulation_1TV.sim", 42);
        nS = sim.getNumStates();
        nA = sim.getNumActions();

        PIG_alg pig(42, nS, nA);
        runPolicy("PIG", &pig, &sim, steps);
    }
    {
        Simulation sim("Simulation_1TV.sim", 42);
        nS = sim.getNumStates();
        nA = sim.getNumActions();

        EPIG_alg epig(42, 10, nS, nA);
        runPolicy("EPIG", &epig, &sim, steps);
    }
    {
        Simulation sim("Simulation_1TV.sim", 42);
        nS = sim.getNumStates();
        nA = sim.getNumActions();

        TPIG_alg tpig(42, nS, nA);
        runPolicy("TPIG", &tpig, &sim, steps);
    }
    {
        Simulation sim("Simulation_1TV.sim", 42);
        nS = sim.getNumStates();
        nA = sim.getNumActions();

        ETPIG_alg etpig(42, 10, nS, nA);
        runPolicy("ETPIG", &etpig, &sim, steps);
    }
    {
        Simulation sim("Simulation_1TV.sim", 42);
        nS = sim.getNumStates();
        nA = sim.getNumActions();

        LP_alg lp(42, nS, nA);
        runPolicy("LP", &lp, &sim, steps);
    }

    cout << "All policies tested." << endl;
    return 0;
}
