#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <set>


#define TIME_PER_FLOOR          10
#define OA_CLASS                0
#define MY_CLASS                1
#define ELEVATOR_TYPE        (OA_CLASS) // Where we assign


enum class direction {
    down = -1,
    idle =  0,
    up   =  1,
};


class OA_Elevator
{
 
    protected:
 
        int m_CurrentFloor;
        std::set<int, std::greater<int>> m_DownFloorsToVisit;
        std::set<int> m_UpFloorsToVisit;
        std::vector<int> m_FloorsToVisit;
        direction m_CurrentDirection;
        int m_TotalTraverseTime;

    public:

        int m_FloorTraversalTime;
        int m_DoorTime;

        // Constructor
        OA_Elevator();

        // Want seperate setters for assert logic in gtest
        void SetCurrentFloor(int floor);

        // Set the current search direction
        void SetSearchDirection(direction CurrentDirection);

        // Set total traverse time
        void SetTotalTraverseTime( int time );

        // Get total traverse time
        int GetTotalTraverseTime( );

        // Add floor to set above current
        void AddUpFloors(int floor);
        
        // Add floor to set below curren
        void AddDownFloors(int floor);

        // Set new floor to visit
        void AddFloorsToVisit(int floor);

        // Clear we visited a floor
        void ClearFloorNotVisited(int floor);

        // Getter methods to contain elevator attributes to class
        int GetCurrentFloor();

        std::vector<int> GetFloorsToVisit();

        void SetFloorsToVisit();

        void SearchBestDirectionNSet();

        // UI
        void RequestUserInput();

        // TODO: need to update current traversal array order
        // Want this to be virtual
        virtual void SetFloorTraversal(){std::cout<<"Base Class Floor Traversal Method" <<std::endl;}

        // TODO: need a requirement to request user info

};
