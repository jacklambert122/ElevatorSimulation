#include "gtest/gtest.h"
#include "ElevatorSimulation.hpp"
#include <algorithm>
#include <limits>
#include <cmath>
#include <map>
#include<ctime>

/* --------------------------------------------------------- */
/* ----------------- Helper Functions ---------------------- */
/* --------------------------------------------------------- */
// Basic psuedo number for range
int RandomNumber(int minNumber, int maxNumber)
{
   return ( rand() % (maxNumber + 1 - minNumber) + minNumber );
}

// Permutation Calaculator for output
int NumPermuations(int n)
{
    int out = n;
    for(int i = n-1; i > 0; i-- )
    {
        out*=i;
    }
    return out;
}

/* --------------------------------------------------------- */
/* -------------- Small Example Test Fixture ----------------*/
/* --------------------------------------------------------- */
class SmallElevatorTest : public ::testing::Test
{
    protected:

    OA_Elevator* elevator;
    int startFloor;
    std::vector<int> FloorsToVisit;

    virtual void SetUp()
    {
        // Set Test Object 
        startFloor = 10;
        FloorsToVisit = { 2, 3, 5, 15, 16};

        // Print out
        std::cout << "+----------------------------------------------" << std::endl;
        std::cout << " Starting Small Test with: " << std::endl;
        std::cout << " StartFloor: " << startFloor << std::endl;
        std::cout << " numFloors: " << FloorsToVisit.size() << std::endl;
        std::cout << " Combinations of floors checked: " << NumPermuations(FloorsToVisit.size()) << std::endl;
        std::cout << "+----------------------------------------------" << std::endl;
    
        // Set elevator object
        elevator = new OA_Elevator();
        elevator->SetCurrentFloor(startFloor);
        for(auto floor : FloorsToVisit)
        {
            elevator->AddFloorsToVisit(floor);
        }
    }
    virtual void TearDown()
    {
        delete elevator;
    }
};

/* --------------------------------------------------------- */
/* ----------- Random Seed Generated Test Fixture -----------*/
/* --------------------------------------------------------- */

class RandomElevatorTest : public ::testing::TestWithParam<int>
{
    protected:

    OA_Elevator* elevator;
    int startFloor;
    std::vector<int> FloorsToVisit;
    int mySeed; // Will figure out --gtest_random_seed later...

    virtual void SetUp()
    {
        // Set Seed 
        mySeed = GetParam();
        //mySeed = 1669046450; 
        srand(mySeed);
        int rand_floor;

        // Main Loop
        int minFloor = 0;
        int maxFloor = 25;
        
        // Init Test Objects
        startFloor = RandomNumber(minFloor, maxFloor);
        int numFloors = (RandomNumber(minFloor, startFloor) > 0) ? RandomNumber(minFloor, startFloor) : 1;

        // Init elevator object
        elevator = new OA_Elevator();
        elevator->SetCurrentFloor(startFloor);

        // Print out
        std::cout << "+----------------------------------------------" << std::endl;
        std::cout << " Starting Random Elevator Test with: " << std::endl;
        std::cout << " Seed: " << mySeed << std::endl;
        std::cout << " StartFloor: " << startFloor << std::endl;
        std::cout << " numFloors: " << numFloors << std::endl;
        std::cout << " Combinations of floors checked: " << NumPermuations(numFloors) << std::endl;
        std::cout << "+----------------------------------------------" << std::endl;

        // Loop and set floors randomly in test && elevator object
        for( int i = 0; i < numFloors; i++)
        {
            rand_floor = RandomNumber(minFloor, startFloor);

            // Add to test object only if unique && not start floor
            if(std::find(FloorsToVisit.begin(), FloorsToVisit.end(), rand_floor) == FloorsToVisit.end()
              && (rand_floor != startFloor ) )
            {
                FloorsToVisit.emplace_back(rand_floor);
            }

            // Set elevator (should handle unique-ness itself and same floor)
            elevator->AddFloorsToVisit(rand_floor);
        }

    }
    virtual void TearDown()
    {
        delete elevator;
    }
};

/* --------------------------------------------------------- */
/* -------------- Small Example Test Fixture ----------------*/
/* --------------------------------------------------------- */

TEST_F(SmallElevatorTest, test_search)
{
    std::vector<int> GetterSet = elevator->GetFloorsToVisit();

    // Brute force approach
    int curTime;
    int dlt;
    std::multimap<int, std::vector<int>> BestTimeMap;
    do {

        // Sum deltas 
        curTime = ( (std::abs(startFloor - *FloorsToVisit.begin())) *  elevator->m_FloorTraversalTime ) + elevator->m_DoorTime; // Need to Add current floor delta to cur total
        for( auto it = FloorsToVisit.begin(); it != FloorsToVisit.end()-1; it++ )
        {   
            auto next = std::next(it);
            dlt = std::abs(*it - *next);
            curTime += ( (dlt * elevator->m_FloorTraversalTime) + elevator->m_DoorTime);
        }

        BestTimeMap.insert(std::pair<int, std::vector<int>> (curTime, FloorsToVisit )); // keep all combos with key as time

    } while ( std::next_permutation(FloorsToVisit.begin(), FloorsToVisit.end()));

    // Keep all combos of best to compare to if there is a tie
    std::vector<std::vector<int>> bestVectorBruteForce;
    int curBest = BestTimeMap.begin()->first;
    auto it = BestTimeMap.begin();

    while( it->first == curBest)
    {
        bestVectorBruteForce.emplace_back(it->second);
        it++;
    }

    elevator->SearchBestDirectionNSet(); // Set the best route
    GetterSet = elevator->GetFloorsToVisit();
    int ElevatorTime = elevator->GetTotalTraverseTime();

    bool bestValid = false;
 
    for(auto& vec : bestVectorBruteForce )
    {
        vec.insert(vec.begin(), startFloor); // Add startFloor
        if( vec == GetterSet )
        {
            bestValid = true;
            break;
        }
    }

    EXPECT_TRUE( bestValid == true) << "Test Failed";
    EXPECT_EQ( curBest, ElevatorTime ) << "Best time: " << curBest
                << "ElevatorTime: " << ElevatorTime << std::endl;
    
}

/* --------------------------------------------------------- */
/* ----------- Random Seed Generated Test Fixture -----------*/
/* --------------------------------------------------------- */
TEST_P(RandomElevatorTest, test_search)
{

    // Brute force approach
    int dlt;
    int curTime;
    std::multimap<int, std::vector<int>> BestTimeMap;
    do {

        // Skip if empty
        if(FloorsToVisit.empty()){curTime = 0; continue;}

        // Sum deltas 
        curTime = ( (std::abs(startFloor - *FloorsToVisit.begin())) *  elevator->m_FloorTraversalTime ) + elevator->m_DoorTime; // Need to Add current floor delta to cur total
        for( auto it = FloorsToVisit.begin(); it != FloorsToVisit.end()-1; it++ )
        {   
            auto next = std::next(it);
            dlt = std::abs(*it - *next);
            curTime += ( (dlt * elevator->m_FloorTraversalTime) + elevator->m_DoorTime);
        }

        // keep all combos with key as time
        BestTimeMap.insert(std::pair<int, std::vector<int>> (curTime, FloorsToVisit )); 

    } while ( std::next_permutation(FloorsToVisit.begin(), FloorsToVisit.end()));

    // Keep all combos of best to compare to if there is a tie
    std::vector<std::vector<int>> bestVectorBruteForce;
    int bestTime = 0;
    if ( !FloorsToVisit.empty() )
    {
        bestTime = BestTimeMap.begin()->first;
        auto it = BestTimeMap.begin();
        while( it->first == bestTime) 
        {
            bestVectorBruteForce.emplace_back(it->second);
            it++;
        }
    }   

    // Elevator totals
    std::vector<int> ElevatorSet = elevator->GetFloorsToVisit();
    elevator->SearchBestDirectionNSet(); // Set the best route
    ElevatorSet = elevator->GetFloorsToVisit();
    int ElevatorTime = elevator->GetTotalTraverseTime();

    // Compare elevator to test object
    bool bestValid = (bestVectorBruteForce.size() > 0) ? false : true; // if empty check time only
    for(auto& vec : bestVectorBruteForce )
    {
        vec.insert(vec.begin(), startFloor); // Add startFloor
        if( vec == ElevatorSet )
        {
            bestValid = true;
            break;
        }
    }

    // Would make an exception handler... this is fine for now
    if ( ( bestValid == false ) || ( bestTime != ElevatorTime )  )
    {
        std::cout << "Test Failed with seed: " << mySeed << std::endl;
        std::cout << "Best Brute Force Time: " << bestTime << std::endl;
        std::cout << "Best Elevator Method Time: " << ElevatorTime << std::endl;
        std::cout << "Best Brute Force Set: ";
        for( auto& vec : bestVectorBruteForce )
        {
            std::cout << "{ ";
            for ( auto& el : vec)
            {
                // last element
                if( el == *(vec.rbegin()) ) { 
                    std::cout << el << " }" << std::endl;
                }else{
                    std::cout << el << ", ";
                }
            }
        }

        std::cout << "Best Elevator Method Set: [ ";
        for ( auto floor : ElevatorSet )
        {
            // last element
            if( (floor) == *(ElevatorSet.rbegin()) ) { 
                std::cout << floor << " ]" << std::endl;
            }else{
                std::cout << floor << ", ";
            }
        }
        
    }

    // What we are testing
    EXPECT_TRUE( bestValid == true );
    EXPECT_EQ( bestTime, ElevatorTime );

}

INSTANTIATE_TEST_SUITE_P( Instantiation, RandomElevatorTest, ::testing::Values(std::time(0), std::time(0)+7, std::time(0)+11, std::time(0)+13));
