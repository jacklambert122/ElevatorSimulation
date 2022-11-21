#include "ElevatorSimulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <set>
#include <cmath>


void PrintElevator(OA_Elevator &elevator);

int main(int argc, char **argv)
{

#if ELEVATOR_TYPE == FASTEST_POSSIBLE
    OA_Elevator e;
#else
    MyElevator e;
#endif

    e.RequestUserInput();

    std::cin.get();


}
