#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>

/* ----------------------------------------------------------------------------------------------
// PreComp Constants
-----------------------------------------------------------------------------------------------*/
//#define SIM_ARR_WIDTH          60
//#define SIM_STR                "*||||||||||||||||||||||||||||||#|||||||||||||||||||||||||||^"
#define TIME_PER_FLOOR         10
#define TIME_VISITING_FLOOR     0
#define OA_CLASS                0
#define MY_CLASS                1
#define ELEVATOR_TYPE        (OA_CLASS) // Where we assign
/* --------------------------------------------------------------------------------------------*/ 

// Direction enum to ensure type
enum class direction {
    down = -1,
    idle =  0,
    up   =  1,
};

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

class OA_Elevator
{
 
    protected:
 
        int m_CurrentFloor;
        int m_TotalTraverseTime;
        std::set<int, std::greater<int>> m_DownFloorsToVisit;
        std::set<int> m_UpFloorsToVisit;
        std::vector<int> m_FloorsToVisit;
        direction m_CurrentDirection;

    public:

        int m_FloorTraversalTime;
        int m_DoorTime;

        // Constructor
        OA_Elevator();

        /* ----------------------------------------------------------------------------------------------
           Setters
           ----------------------------------------------------------------------------------------------*/

        // Want seperate setters for assert logic in gtest
        void SetCurrentFloor(int floor);

        // Set the current search direction
        void SetSearchDirection(direction CurrentDirection);

        // Set total traverse time
        void SetTotalTraverseTime( int time );

        // Sets floors to visit
        void SetFloorsToVisit();

        /* ----------------------------------------------------------------------------------------------
           Getters
           ----------------------------------------------------------------------------------------------*/

        // Get best floors to visit
        std::vector<int> GetFloorsToVisit();

        // Get total traverse time
        int GetTotalTraverseTime();

        // Getter methods to contain elevator attributes to class
        int GetCurrentFloor();

        /* ----------------------------------------------------------------------------------------------
          Calculators
          ----------------------------------------------------------------------------------------------*/

        // Add floor to set above current
        void AddUpFloors(int floor);
        
        // Add floor to set below curren
        void AddDownFloors(int floor);

        // Add new floor to visit to single vector
        void AddFloorsToVisit(int floor);

        // Does the grunt work of searching and setting 
        void SearchBestDirectionNSet();

        /* ----------------------------------------------------------------------------------------------
           Interfaces
           ----------------------------------------------------------------------------------------------*/

        // Helper Function to Print the Simulated Traveral
        void printProgress(float indRatio, int width);

        // Simulate Elevator Traversal on cmd line
        void SimulateElevatorTraversal();

        // Print best traversal and total time
        void PrintBestFloorTraversal();

        // Request user input
        void RequestUserInput();

};

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