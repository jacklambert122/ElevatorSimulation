#include "ElevatorSimulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <iterator>
#include <set>
#include <cmath>

/* -----------------------------------------------------------------------------------------------
Class to represent what I think was asked:
    Purpose:
        - no floor inputs and will find the fastest route
    Assumptions:
        - The elevator works by requesting user input for current floor and floors to visit

    Inputs:
        - Starting floor
        - floors to visit
    
    outputs:
        - Total travel time
        - Floors visited 
-------------------------------------------------------------------------------------------------*/
// Default Constructor - Initalize so we dont ref unkown memory
OA_Elevator::OA_Elevator() : m_CurrentFloor(0), m_FloorTraversalTime(10), m_DoorTime(0), m_TotalTraverseTime(0) { }

// Want seperate setters for assert logic in gtest
// Will need to be copy types rather than by ref 
void OA_Elevator::SetCurrentFloor(int floor)
{
    m_CurrentFloor = floor;
}

void OA_Elevator::SetSearchDirection(direction CurrentDirection)
{
    m_CurrentDirection = CurrentDirection;
}

void OA_Elevator::SetTotalTraverseTime( int time )
{
    m_TotalTraverseTime = time;
}

int OA_Elevator::GetTotalTraverseTime()
{
    return m_TotalTraverseTime;
}

void OA_Elevator::AddUpFloors(int floor)
{ 
    m_UpFloorsToVisit.insert( floor );
}

void OA_Elevator::AddDownFloors(int floor)
{ 
    m_DownFloorsToVisit.insert( floor );
}

// Want to update floors as we go so if we add functionallity to insert floors upon request
void OA_Elevator::AddFloorsToVisit(int floor)
{ 
    if( floor < m_CurrentFloor ){ AddDownFloors( floor ); }
    else if ( floor > m_CurrentFloor ){ AddUpFloors( floor ); } 
    else { SetSearchDirection( direction::idle ); }
}

// void OA_Elevator::ClearFloorNotVisited(int floor)
// {
//     m_FloorsToVisit.insert( std::pair<int, visited>(floor, visited::floor_not_visited) );
// }

int OA_Elevator::GetCurrentFloor()
{
    return m_CurrentFloor;
}

std::vector<int> OA_Elevator::GetFloorsToVisit()
{
    return m_FloorsToVisit;
}

void OA_Elevator::SetFloorsToVisit()
{

    if ( m_CurrentDirection == direction::up )
    {
        for (auto floor : m_UpFloorsToVisit)
        {
            m_FloorsToVisit.push_back(floor);
        }
        for (auto floor : m_DownFloorsToVisit)
        {
            m_FloorsToVisit.push_back(floor);
        }
    }
    else
    {
        for (auto floor : m_DownFloorsToVisit)
        {
            m_FloorsToVisit.push_back(floor);
        }
        for (auto floor : m_UpFloorsToVisit)
        {
            m_FloorsToVisit.push_back(floor);
        }
    }

}


// At current floor we will have to search all nodes in each direction for best
// Keep counts for implementing door opening/closing times to consider
void OA_Elevator::SearchBestDirectionNSet()
{ 
    direction curBest;
    int upTime;
    int downTime;
    int linkTime = 0;
    int upCount = m_UpFloorsToVisit.size();
    int downCount = m_DownFloorsToVisit.size();

    // Set iterator so we dont duplicate code...
    int upEnd = (upCount==0) ? (m_CurrentFloor) : *m_UpFloorsToVisit.rbegin();
    int downEnd = (downCount==0) ? (m_CurrentFloor) : *m_DownFloorsToVisit.rbegin();
    bool needsLink = ( (upCount>0) && (downCount>0) ) ?  true : false;

    // Time in any direction is just the time from current to bottom/top floor
    upTime   = (upEnd  - m_CurrentFloor) * m_FloorTraversalTime;
    downTime = (m_CurrentFloor - downEnd) * m_FloorTraversalTime;

    // If we consider loading times... well need this
    upTime   += (upCount  * m_DoorTime);
    downTime += (downCount  * m_DoorTime);

    // If tied - take whichever has fewer counts
    // Allows case with 0 floor time to still pick the correct path (not necessary but make debug easier)
    curBest = ((downTime <= upTime) && (downCount < upCount) && (downCount != 0)) ? direction::down : direction::up;
    if ( needsLink )
    {   
        linkTime = ( curBest == direction::down) ? (upEnd - *m_DownFloorsToVisit.begin()) : std::abs(downEnd - *m_UpFloorsToVisit.begin());
    }

    // Setter methods here
    SetSearchDirection(curBest);
    SetFloorsToVisit();
    SetTotalTraverseTime(upTime + downTime + linkTime);

}

void OA_Elevator::RequestUserInput()
{
    // Locals
    std::string s_Input;
    std::string floor;
    bool exit = false;
    while (!exit)
    {
        // Retrieve Current Floor
        std::cout << "+========= Main Menu =========+" << std::endl;
        std::cout << " Enter Starting Floor: ";
        std::getline(std::cin, s_Input);
        SetCurrentFloor( stoi(s_Input) ); // Set Current Floor

        // Retrieve Floors to visit
        std::cout << "Enter Floors to Visit: ";
        std::getline(std::cin, s_Input);
        std::istringstream ss(s_Input);

        while (ss >> floor)
        {
            try
            {
                AddFloorsToVisit( stoi(floor) ); // Convert to int and add in order (if unique)
                exit = true;
            }
            catch( std::exception& e)
            {
                std::cout << "Non-numeric Input... Will wrong inputs handle later" << std::endl;

            }
        }
        std::cout << "+=============================+" << std::endl;

        if ( m_UpFloorsToVisit.empty() && m_DownFloorsToVisit.empty() )
        {
            std::cout << "Must enter floors!";
        }

        SearchBestDirectionNSet(); // Search and set best direction 
    }

}



/* -----------------------------------------------------------------------------------------------
Class that does what I think an elevator simulation should do :
    Purpose:
        - Multiple riders each requesting one floor each
        - First request a direction then once "picked up" we add requested info
            - Rider : (current floor, floor requested)
                - (current < req ) ? down : up
        - travels like a normal elevator by traversing in a set direction until no more passengers req

    Assumptions:
        - Starts at base level
-------------------------------------------------------------------------------------------------*/
class MyElevator
{

};


