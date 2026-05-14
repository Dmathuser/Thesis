#include <iostream>
#include "simulation.h"
#include "simLogger.h"
#include "TPIG.h"
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

int main()
{
    int nS, nA, steps, i;
    steps = 100;

    cout << "Testing with SimLogger..." << endl;

    Simulation sim("Simulation_1TV.sim", 42);
    nS = sim.getNumStates();
    nA = sim.getNumActions();

    TPIG_alg policy(42, nS, nA);
    SimLogger logger(&sim, &policy, "Simulation_1TV.sim", sim.getSeed());

    sim.setCurState(5);
    State s = {5, 0};
    Action a = UP;
    StateTransition sas = {0};

    logger.startLog();
    for (i = 0; i < steps; i++) {
        logger.log();
        a = policy.getAction(s);
        sim.moveState(a);
        sas = getSAS(s, a, &sim);
        policy.Update(sas);
        s = sas.sPrime;
    }
    logger.stopLog();
    logger.printSimLogs("/tmp/valgrind_test_out.txt");

    cout << "Done." << endl;
    return 0;
}
