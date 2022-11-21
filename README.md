
# Elevator Simulation

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

## Purpose
Given the current state of an elevator, find the path to traverse such that the time is at a minimum. The best search path assumes each floor takes the same amount of time to traverse and that the time visiting a floor takes zero time. The class for elevator allows for the time visiting a floor to be non-zero. By default, the search will select the direction where the minimum total time is acheived by checking which furthest floor in a given direction is closest and multiplying that by the floor traversal time. When non-zero inputs are placed into the time while visiting a floor (say for opening the door), the search uses the counts in each direction (up/down) and multiplies the floor visit time and then adds it to the given directions furthest floor times the floor traveral time.

The class was constructed to handle forecasted inputs. The current inputs represent the elevator being in a state already based on inputs from other riders. However, it allows for requested floors to update the current state by floor. The floors update by adding the floors to one of two sets. The "m_UpFloorsToVisit" set represents the floors above the start floor in acending order and the "m_DownFloorsToVisit" set represents the floors below the start floor in decending order. This allows for adding each floor in O(log(N)) as sets assume order and can be binary searched. This allows there to be a predetermined order which elevators use. The accending in a single direction until all in that direction are gone is also a subset of the optimal solution. Once the best direction is choosen based on the current state of the elevate from the inputs, it then stitches these sets together to give the fastest output vector. In practice this would be done faster without stitching and just using the two sets independently, however this was done for a consise output of a single vector.

*Note - When floor visiting time is zero, and tie for which direction to search is broken by whichever direction has a higher count, but this is not necessary since both directions are valid returns.

### Inputs
- Start floor
- List of floors to visit (must be space separate - [ num1, num2 ] or [ num1 num2 ] )

### Outputs
- Fastest traversal time
- Floors visited in fastested traversal order

## Google Test Setup
Google test was used to test current implementation against a brute force search for the lowest total time floor traversal. This test creates a simple test fixture for a small example to run first as a pre-run before running a random test paramaterized test fixure that uses random seeds to generage a random creation of inputs. For both tests the output of the elevator method is compared against the brute force search. The brute force best time search looks through all permutations of the input list to looks for the minmum time. In the case there are multiple traversal lists with the best time - it loops to see if ANY match the elevators method. Since the brute force method take O(N!) - the range for floors should be left low. The paramterized testing on the random workload generator was intialized to run 4 times each with the current time plus some offset to make unique seeds for each, each time test is called. This can be changed to be more dynamic in the future, by having Jenkins poll test runs.

## Project Setup:
```sh
git clone "this url"

```

## Google Test Setup:
```sh
mkdir lib
cd lib
git clone https://github.com/google/googletest.git
cd googletest 
mkdir install 
cd install 
cmake ../  #creates a make file 
make #compiles Google Test
sudo make install #installs Google Test
echo "export CPLUS_INCLUDE_PATH=/usr/local/include" >> ~/.bash_profile
echo "export LIBRARY_PATH=/usr/local/lib" >> ~/.bash_profile
source ~/.bash_profile 
```

## CMAKE Build:

### For Mac: 
```sh
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"
make all
```
### run:
```sh
./src/ElevatorSimulation_run
```
### test:
```sh
./test/ElevatorSimulation_tst
```
### For VS code: 
Default cmake generator is set to ninja with cmake tools extention... but from command line select which ever you like for build dir!

#### ex: 
```sh
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -G "[generator]"
make all
```



