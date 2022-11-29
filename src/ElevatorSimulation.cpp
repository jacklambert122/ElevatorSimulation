#include "ElevatorSimulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <set>
#include <cmath>
#include <thread>

// Default Constructor - Initalize so we dont ref unkown memory
OA_Elevator::OA_Elevator() : m_CurrentFloor(0), m_FloorTraversalTime(TIME_PER_FLOOR), m_DoorTime(TIME_VISITING_FLOOR), m_TotalTraverseTime(0) { }

// Want seperate setters for assert logic in gtest
// Will need to be copy types rather than by ref
/* ----------------------------------------------------------------------------------------------
  Setters
  ----------------------------------------------------------------------------------------------*/
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

void OA_Elevator::SetFloorsToVisit()
{
    m_FloorsToVisit.push_back(m_CurrentFloor);
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

/* ----------------------------------------------------------------------------------------------
  Getters
  ----------------------------------------------------------------------------------------------*/

int OA_Elevator::GetTotalTraverseTime()
{
    return m_TotalTraverseTime;
}

int OA_Elevator::GetCurrentFloor()
{
    return m_CurrentFloor;
}

std::vector<int> OA_Elevator::GetFloorsToVisit()
{
    return m_FloorsToVisit;
}

/* ----------------------------------------------------------------------------------------------
  Calculations
  ----------------------------------------------------------------------------------------------*/

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
        linkTime = ( curBest == direction::down) ? (m_CurrentFloor - downEnd) : (upEnd - m_CurrentFloor) ;
    }

    // Adding link floor time and travel time scalar
    linkTime *= m_FloorTraversalTime;
    linkTime += m_DoorTime;

    // Setter methods here
    SetSearchDirection( curBest );
    SetFloorsToVisit();
    SetTotalTraverseTime( upTime + downTime + linkTime );

}

/* ----------------------------------------------------------------------------------------------
  Interface
  ----------------------------------------------------------------------------------------------*/

void OA_Elevator::printProgress(float ind, const int WIDTH)
{
    std::string arrowString = "^";
    int lPad = ind - 1;
    int rPad = WIDTH - lPad;
    std::string lBlanks(lPad, ' ');
    std::string rBlanks(rPad, ' ');
    std::string outString = lBlanks + arrowString + rBlanks;
    printf("\r %*s", WIDTH, outString.c_str());
    fflush(stdout); // no newline so force it
}

void OA_Elevator::SimulateElevatorTraversal()
{

    // Sort for elevator display
    std::vector<int> sortedVector(m_FloorsToVisit);
    sort(sortedVector.begin(), sortedVector.end());

    // String stream 
    std::stringstream ss;
    ss << " [ ";
    for ( auto floor : sortedVector )
    {
        // last element
        if( (floor) == *sortedVector.rbegin())  { 
            ss << floor << " ]";
        }else{
            ss << floor << " ";
        }
    }

    // Variables
    float ind;
    std::string sFloor;
    std::string ssToStr; 

    // String stream to string to find index in stream
    ssToStr = ss.str();
    const int STR_SIZE = ssToStr.size();
    
    // Elevator Simulation print out
    std::cout << std::endl << "Elevator Simulated Cmdline Traversal: " << std::endl;
    std::cout << ssToStr <<std::endl;

    // Display in fastest traversal order
    for ( auto floor : m_FloorsToVisit )
    {
        sFloor = std::to_string(floor);
        ind = ssToStr.find( sFloor );
        printProgress(ind, STR_SIZE);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    std::cout << std::endl;

}

void OA_Elevator::PrintBestFloorTraversal()
{
    std::cout << " Total Traversal Time: " << GetTotalTraverseTime() << std::endl;
    std::cout << " Best Traversal: [ ";
    for ( auto floor : GetFloorsToVisit() )
    {
        // last element
        if( (floor) == *(GetFloorsToVisit().rbegin()) ) { 
            std::cout << floor << " ]" << std::endl;
        }else{
            std::cout << floor << ", ";
        }
    }

}

void OA_Elevator::RequestUserInput()
{
    // Locals
    std::string sInput;
    std::string floor;
    bool exit = false;
    while (!exit)
    {
        // Retrieve Current Floor
        std::cout << "+========= Main Menu =========+" << std::endl;
        std::cout << " Enter Starting Floor: ";
        std::getline(std::cin, sInput);
        SetCurrentFloor( stoi(sInput) ); // Set Current Floor

        // Retrieve Floors to visit
        std::cout << " Enter Floors to Visit: ";
        std::getline(std::cin, sInput);
        std::istringstream ss(sInput);

        while (ss >> floor)
        {
            try
            {
                AddFloorsToVisit( stoi(floor) ); // Convert to int and add in order (if unique)
                exit = true;
            }
            catch( std::exception& e)
            {
                std::cout << " Non-numeric Input... Will wrong inputs handle later" << std::endl;

            }
        }

        // Some Input Handleing - would add more
        if ( m_UpFloorsToVisit.empty() && m_DownFloorsToVisit.empty() )
        {
            std::cout << " Must enter floors with spaces in-between!" << std::endl;
        }

        SearchBestDirectionNSet(); // Search and set best direction 
        PrintBestFloorTraversal(); // Print the Best Found
        SimulateElevatorTraversal();

        std::cout << "+=============================+" << std::endl;

    }

}
