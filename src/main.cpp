#include "ElevatorSimulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <iterator>

void PrintElevator(OA_Elevator &elevator);

int main(int argc, char **argv)
{

#if ELEVATOR_TYPE == FASTEST_POSSIBLE
    OA_Elevator e;
#else
    MyElevator e;
#endif

    e.RequestUserInput();
    PrintElevator(e);

    std::cin.get();


}

// For Debug
void PrintElevator(OA_Elevator &elevator)
{
    std::cout << "Current Floor in Set: [ ";
    for ( auto floor : elevator.GetFloorsToVisit() )
    {
        // last element
        if( (floor) == *(elevator.GetFloorsToVisit().rbegin()) ) { 
            std::cout << floor << " ]" << std::endl;
        }else{
            std::cout << floor << ", ";
        }
    }
}