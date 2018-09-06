//
//  EightPuzzle.h
//  8puzzle
//
//  Created by jing hong chen on 9/2/18.
//  Copyright © 2018 jing hong chen. All rights reserved.
//

#ifndef EightPuzzle_h
#define EightPuzzle_h
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <algorithm>  

#define CLOCKS_PER_MS (CLOCKS_PER_SEC/1000)
#define PUZZLE_SIZE 9
#define ROW_SIZE 3
#define MAX_STEPS 50

const std::string PUZZLE_GOAL = "12345678_";

enum Moves { UP, DOWN, LEFT, RIGHT };

struct Nodes {
    std::string currentState;
    std::vector<Moves> currentPath;
    int distance;
};

struct checkByDistance
{
    bool operator()(const Nodes& lhs, const Nodes& rhs) const
    {
        return lhs.distance > rhs.distance;
    }
};

class EightPuzzle {
    private:
    std::string startState;  // a 9 charater string representing the state of puzzle, first three character representing the first row of the puzzle, and so on.
    bool solutionFound;
    std::vector<Moves> solutionSteps;  // a list of solution steps how puzzle move from the start to the goal
    std::map<std::string, unsigned> visitedStates;  // record visited node state and the current cost to reach it
    int createdNodes;
    int expandedNodes;
    std::clock_t clock();
    std::clock_t start;
    std::clock_t end;
    long solveTime;  //in milliseconds

    public:
    EightPuzzle() {
        startState = PUZZLE_GOAL;
    }
    
    /**
     * get puzzle starting state from a file, the file should only contain 9 charaters in an appropriate order
     */
    void puzzleInit(std::string fileName) {
        std::ifstream inputFile(fileName);
        if (inputFile) {
            std::string tempStr;
            std::stringstream ss(std::ios::in | std::ios::out);
            //ignore all the whitespace
            while (inputFile >> tempStr) {
                ss << tempStr;
            }
            std::string inputStr = ss.str();
            // check if the input state is valid
            if (inputStr.size()==9
                && inputStr.find('1') != std::string::npos
                && inputStr.find('2') != std::string::npos
                && inputStr.find('3') != std::string::npos
                && inputStr.find('4') != std::string::npos
                && inputStr.find('5') != std::string::npos
                && inputStr.find('6') != std::string::npos
                && inputStr.find('7') != std::string::npos
                && inputStr.find('8') != std::string::npos
                && inputStr.find('_') != std::string::npos) {
                startState = inputStr;
            } else {
                std::cout << "The file " << fileName << " doesn't contain a correct puzzle state." << std::endl;
            }
        } else {
            std::cout << "Cannot open the file " << fileName << std::endl;
        }
    }
    
    /**
     * get a puzzle state, then return the position of the 'space'
     */
    int checkSpacePosition(std::string state) {
        int position = -1;
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            if (state[i] == '_') {
                position = i;
            }
        }
        return position;
    }
    
    /**
     * return a string representing the correct direction massage for result print out
     * changed the state string directly if the movement is appropriate
     */
    std::string checkSolutionSteps(std::string& state, Moves direction) {
        std::string movementResult = "";
        int switchPosstion = -1;
        char tempChar;
        // get the posistion of the 'space'
        int spacePosition = checkSpacePosition(state);
        switch (direction) {
            case UP:
            if (spacePosition < ROW_SIZE) {
                movementResult = "STAY ";
                // if the movement doesn't make sense, say 'stay'. (technically shouldn't happen). Similiarly below.
            } else {
                movementResult = " UP  ";
                // change the state string (moving 'up'). Similiarly below.
                switchPosstion = spacePosition - ROW_SIZE;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case DOWN:
            if (spacePosition >= PUZZLE_SIZE - ROW_SIZE) {
                movementResult = "STAY ";
            } else {
                movementResult = "DOWN ";
                switchPosstion = spacePosition + ROW_SIZE;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case LEFT:
            if ((spacePosition % ROW_SIZE) == 0) {
                movementResult = "STAY ";
            } else {
                movementResult = "LEFT ";
                switchPosstion = spacePosition - 1;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case RIGHT:
            if ((spacePosition % ROW_SIZE) == ROW_SIZE - 1) {
                movementResult = "STAY ";
            } else {
                movementResult = "RIGHT";
                switchPosstion = spacePosition + 1;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
        }
        return movementResult;
    }
    
    /**
     * get puzzle state and the direction subjected to move, change the state string directedly if the movement is reasonable
     */
    void puzzleMove(std::string& state, Moves dir) {
        int switchPosstion = -1;
        char tempChar;
        // get the posistion of the 'space'
        int spacePosition = checkSpacePosition(state);
        switch (dir) {
            case UP:
            if (spacePosition >= ROW_SIZE) {
                // if the movement is appropriate, change the state string (moving 'up'). Similiarly below.
                switchPosstion = spacePosition - ROW_SIZE;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case DOWN:
            if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                switchPosstion = spacePosition + ROW_SIZE;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case LEFT:
            if ((spacePosition % ROW_SIZE) != 0) {
                switchPosstion = spacePosition - 1;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case RIGHT:
            if ((spacePosition % ROW_SIZE) != ROW_SIZE - 1) {
                switchPosstion = spacePosition + 1;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
        }
    }
    
    /**
     * print out searching result of different strategies into a txt file named after the search strategy name
     */
    void printSolution(std::string searchName) {
        std::string currentState = startState;
        std::ofstream myfile;
        myfile.open (searchName + "_result.txt");
        myfile << "search strategy: " << searchName << std::endl << std::endl;
        myfile << "Running time (in milliseconds): "  << solveTime << std::endl << std::endl;
        myfile << "Created Nodes: "  << createdNodes << std::endl << std::endl;
        myfile << "Expanded Nodes: "  << expandedNodes << std::endl << std::endl;
        myfile << "Solution Steps: "  << solutionSteps.size() << std::endl << std::endl;
        myfile << "                 " << currentState[0] << currentState[1] << currentState[2] << std::endl;
        myfile << "Starting puzzle: " << currentState[3] << currentState[4] << currentState[5] << std::endl;
        myfile << "                 " << currentState[6] << currentState[7] << currentState[8] << std::endl;
        if (!solutionFound) {
            myfile << std::endl << "No solution was found by this search strategy." << std::endl;
        } else {
            std::string movement;
            for (int i = 0; i < solutionSteps.size(); i++) {
                movement = checkSolutionSteps(currentState, solutionSteps[i]);
                myfile << std::endl;
                myfile << "                 " << currentState[0] << currentState[1] << currentState[2] << std::endl;
                myfile << " --- " << movement <<" --->  " << currentState[3] << currentState[4] << currentState[5] << std::endl;
                myfile << "                 " << currentState[6] << currentState[7] << currentState[8] << std::endl;
            }

        }
        myfile.close();
    }
    
    /**
     * conduct Breadth First Search to the puzzle start state, store the result the vector 'solutionSteps'.
     * the vector would be empty if no solution was found.
     * print out the result to a txt file at the end.
     */
    void breadthFirst() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        visitedStates.clear();
        solutionFound = false;
        // using a queue to implement BFS
        std::queue<Nodes> nodeQueue;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            std::vector<Moves> path;
            Nodes startNode{startState, path, 0};
            nodeQueue.push(startNode);  // initial the checking queue
            visitedStates.insert(std::pair<std::string, unsigned>(startState, 1));
            while (!solutionFound && !nodeQueue.empty()) {
                expandedNodes++;
                visitedStates.insert(std::pair<std::string, unsigned>(nodeQueue.front().currentState, 1));
               if (nodeQueue.front().currentState.compare(PUZZLE_GOAL) == 0) {
                    // found a solution here!
                    solutionFound = true;
                    solutionSteps = nodeQueue.front().currentPath;
                } else {
                    // if not found a solution, add its child nodes into the queue
                    // insert current node to vistied map
                    int spacePosition = checkSpacePosition(nodeQueue.front().currentState);
                    if (spacePosition >= ROW_SIZE) {
                        // if that state can move 'up', add the 'up' state as its child node. Similiarly below.
                        Nodes newNode{nodeQueue.front().currentState, nodeQueue.front().currentPath, 0};
                        puzzleMove(newNode.currentState, UP);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            // if the child node is not a visited state, add it to the queue.
                            newNode.currentPath.push_back(UP);
                            nodeQueue.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        Nodes newNode{nodeQueue.front().currentState, nodeQueue.front().currentPath, 0};
                        puzzleMove(newNode.currentState, DOWN);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(DOWN);
                            nodeQueue.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        Nodes newNode{nodeQueue.front().currentState, nodeQueue.front().currentPath, 0};
                        puzzleMove(newNode.currentState, LEFT);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(LEFT);
                            nodeQueue.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        Nodes newNode{nodeQueue.front().currentState, nodeQueue.front().currentPath, 0};
                        puzzleMove(newNode.currentState, RIGHT);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(RIGHT);
                            nodeQueue.push(newNode);
                            createdNodes++;
                        }
                    }
                    nodeQueue.pop();  // pop the current node from the queue
                }
            }
        }
        end = std::clock();
//        solveTime = (end - start) / CLOCKS_PER_MS;
        solveTime = visitedStates.size();
        printSolution("breadth_first");
    }
    
    /**
     * conduct depth First Search to the puzzle start state, store the result the vector 'solutionSteps'.
     * the vector would be empty if no solution was found.
     * print out the result to a txt file at the end.
     */
    void depthFirst() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        visitedStates.clear();
        solutionFound = false;
        // using a stack to implement DFS
        std::stack<Nodes> nodeStack;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            std::vector<Moves> path;
            Nodes startNode{startState, path, 0};
            nodeStack.push(startNode);  // initial the checking stack
            createdNodes++;
            while (!solutionFound && !nodeStack.empty()) {
                expandedNodes++;
                if (nodeStack.top().currentState.compare(PUZZLE_GOAL) == 0) {
                    // found a solution here!
                    solutionFound = true;
                    solutionSteps = nodeStack.top().currentPath;
                } else {
                    // if not found a solution, add its child nodes into the stack
                    std::string currentState =nodeStack.top().currentState;
                    std::vector<Moves> currentPath = nodeStack.top().currentPath;
                    nodeStack.pop();  // pop the top node first
                    // insert current node to vistied map
                    visitedStates.insert(std::pair<std::string, unsigned>(currentState, 0));
                    int spacePosition = checkSpacePosition(currentState);
                    if (spacePosition >= ROW_SIZE) {
                        // if that state can move 'up', add the 'up' state as its child node. Similiarly below.
                        Nodes newNode{currentState, currentPath, 0};
                        puzzleMove(newNode.currentState, UP);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            // if the child node is a not visited state, add it to the stack.
                            newNode.currentPath.push_back(UP);
                            nodeStack.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        Nodes newNode{currentState, currentPath, 0};
                        puzzleMove(newNode.currentState, DOWN);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(DOWN);
                            nodeStack.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        Nodes newNode{currentState, currentPath, 0};
                        puzzleMove(newNode.currentState, LEFT);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(LEFT);
                            nodeStack.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        Nodes newNode{currentState, currentPath, 0};
                        puzzleMove(newNode.currentState, RIGHT);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(RIGHT);
                            nodeStack.push(newNode);
                            createdNodes++;
                        }
                    }
                }
            }
        }
        end = std::clock();
        solveTime = (end - start) / CLOCKS_PER_MS;
        printSolution("depth_first");
    }

    /**
     * a recursive called function for Depth Limited Search.
     * get a before-move state and a movement direction, return ture if found a solution in this movement
     */
    bool depthLimitedSearch(std::string state, Moves direction, int depth) {
        createdNodes++;
        expandedNodes++;
        if (depth > MAX_STEPS) {
            // if the searching is deeper than the limit, stop and return false.
            return false;
        } else {
            puzzleMove(state, direction);  //move the puzzle
            if (state.compare(PUZZLE_GOAL) == 0) {
                // if it is the goal state, return true.
                return true;
            } else if (visitedStates.find(state) != visitedStates.end() && visitedStates[state] <= depth) {
                // if it is a visited state, and the reach path cost is less or equal before, stop expanding this state
                return false;
            } else {
                // otherwise, expand this node
                // insert a new visited node, or update its cost if already existed
                visitedStates[state] = depth;
                int spacePosition = checkSpacePosition(state);
                // if the puzzle state can go 'up', go search if there is a solution in this direction. Similiar below.
                if (spacePosition >= ROW_SIZE && depthLimitedSearch(state, UP, depth + 1)) {
                    // found a solution here!
                    solutionSteps.push_back(UP);
                    return true;
                } else if ((spacePosition < PUZZLE_SIZE - ROW_SIZE) && depthLimitedSearch(state, DOWN, depth + 1)) {
                    solutionSteps.push_back(DOWN);
                    return true;
                } else if ((spacePosition % ROW_SIZE != 0) && depthLimitedSearch(state, LEFT, depth + 1)) {
                    solutionSteps.push_back(LEFT);
                    return true;
                } else if ((spacePosition % ROW_SIZE != ROW_SIZE - 1) && depthLimitedSearch(state, RIGHT, depth + 1)) {
                    solutionSteps.push_back(RIGHT);
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    
    /**
     * conduct Depth Limited Search to the puzzle start state, store the result the vector 'solutionSteps'.
     * the vector would be empty if no solution was found.
     * print out the result to a txt file at the end.
     */
    void depthLimited() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        visitedStates.clear();
        solutionFound = true;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            visitedStates.insert(std::pair<std::string, unsigned>(startState, 0));
            int spacePosition = checkSpacePosition(startState);
            // if the puzzle state can go 'up', go search if there is a solution in this direction. Similiar below.
            if (spacePosition >= ROW_SIZE && depthLimitedSearch(startState, UP, 1)) {
                // found a solution here!
                solutionSteps.push_back(UP);
            } else if ((spacePosition < PUZZLE_SIZE - ROW_SIZE) && depthLimitedSearch(startState, DOWN, 1)) {
                solutionSteps.push_back(DOWN);
            } else if ((spacePosition % ROW_SIZE != 0) && depthLimitedSearch(startState, LEFT, 1)) {
                solutionSteps.push_back(LEFT);
            } else if ((spacePosition % ROW_SIZE != ROW_SIZE - 1) && depthLimitedSearch(startState, RIGHT, 1)) {
                solutionSteps.push_back(RIGHT);
            } else {
                solutionFound = false;
            }
        }
        end = std::clock();
        solveTime = (end - start) / CLOCKS_PER_MS;
        // since it is recursive process, the solution step is in reversed order stored. flip it first.
        std::reverse(solutionSteps.begin(),solutionSteps.end());
        printSolution("depth_limited");
    }

    /**
     * a recursive called function for Iterative Deepening Search.
     * get a before-move state and a movement direction, as well as the current depth and limited depth
     * return ture if found a solution in this movement
     */
    bool iterativeDeepeningSearch(std::string state, Moves direction, int start_depth, int max_depth) {
        createdNodes++;
        expandedNodes++;
        if (start_depth > max_depth) {
            // if the searching is deeper than the limit, stop and return false.
            return false;
        } else {
            puzzleMove(state, direction);  //move the puzzle
            if (state.compare(PUZZLE_GOAL) == 0) {
                // if it is the goal state, return true.
                return true;
            } else if (visitedStates.find(state) != visitedStates.end() && visitedStates[state] <= start_depth) {
                // if it is a visited state, and the reach path cost is less or equal before, stop expanding this state
                return false;
            } else {
                // insert a new visited node, or update its cost if already existed
                visitedStates[state] = start_depth;
                int spacePosition = checkSpacePosition(state);
                // if the puzzle state can go 'up', go search if there is a solution in this direction. Similiar below.
                if (spacePosition >= ROW_SIZE && iterativeDeepeningSearch(state, UP, start_depth + 1, max_depth)) {
                    // found a solution here!
                    solutionSteps.push_back(UP);
                    return true;
                } else if ((spacePosition < PUZZLE_SIZE - ROW_SIZE) && iterativeDeepeningSearch(state, DOWN, start_depth + 1, max_depth)) {
                    solutionSteps.push_back(DOWN);
                    return true;
                } else if ((spacePosition % ROW_SIZE != 0) && iterativeDeepeningSearch(state, LEFT, start_depth + 1, max_depth)) {
                    solutionSteps.push_back(LEFT);
                    return true;
                } else if ((spacePosition % ROW_SIZE != ROW_SIZE - 1) && iterativeDeepeningSearch(state, RIGHT, start_depth + 1, max_depth)) {
                    solutionSteps.push_back(RIGHT);
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    
    /**
     * conduct Iterative Deepening Search to the puzzle start state, store the result the vector 'solutionSteps'.
     * based on Limited DFS but iteratively changing the limited depth.
     * the vector would be empty if no solution was found.
     * print out the result to a txt file at the end.
     */
    void iterativeDeepening() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        solutionFound = false;
        int current_max_depth = 0;  // depth limit
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            int spacePosition = checkSpacePosition(startState);
            while (!solutionFound) {
                visitedStates.clear();  // reflash the visited nodes set in every iteration
                visitedStates.insert(std::pair<std::string, unsigned>(startState, 0));
                current_max_depth++;  // iteratively increasing depth limit
                // if the puzzle state can go 'up', go search if there is a solution in this direction. Similiar below.
                if (spacePosition >= ROW_SIZE && iterativeDeepeningSearch(startState, UP, 1, current_max_depth)) {
                    // found a solution here!
                    solutionSteps.push_back(UP);
                    solutionFound = true;
                } else if ((spacePosition < PUZZLE_SIZE - ROW_SIZE) && iterativeDeepeningSearch(startState, DOWN, 1, current_max_depth)) {
                    solutionSteps.push_back(DOWN);
                    solutionFound = true;
                } else if ((spacePosition % ROW_SIZE != 0) && iterativeDeepeningSearch(startState, LEFT, 1, current_max_depth)) {
                    solutionSteps.push_back(LEFT);
                    solutionFound = true;
                } else if ((spacePosition % ROW_SIZE != ROW_SIZE - 1) && iterativeDeepeningSearch(startState, RIGHT, 1, current_max_depth)) {
                    solutionSteps.push_back(RIGHT);
                    solutionFound = true;
                }
            }
        }
        end = std::clock();
        solveTime = (end - start) / CLOCKS_PER_MS;
        // since it is recursive process, the solution step is in reversed order stored. flip it first.
        std::reverse(solutionSteps.begin(),solutionSteps.end());
        printSolution("iterative_deepening");
    }

    /**
     * conduct Bidirectional Search (based on BFS) to the puzzle start state, store the result the vector 'solutionSteps'.
     * the vector would be empty if no solution was found.
     * print out the result to a txt file at the end.
     */
    void bidirectionalSearch() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        solutionFound = false;
        // nodes in check list of two direction at the same level (searching depth)
        std::queue<Nodes> startSideCheckList;
        std::queue<Nodes> goalSideCheckList;
        // nodes waiting to be expanded of two direction
        std::queue<Nodes> startSideWaitList;
        std::queue<Nodes> goalSideWaitList;
        // two sets contain connected nodes to two roots
        std::map<std::string, std::vector<Moves>> startConnectedNodes;
        std::map<std::string, std::vector<Moves>> goalConnectedNodes;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            std::vector<Moves> path;
            Nodes startNode{startState, path, 0};
            Nodes goalNode{PUZZLE_GOAL, path, 0};
            // initial two checking lists
            startSideCheckList.push(startNode);
            goalSideCheckList.push(goalNode);
            while (!solutionFound && !(startSideCheckList.empty() && goalSideCheckList.empty())) {
                // check start side
                while (!startSideCheckList.empty()) {
                    expandedNodes++;
                    // compare each nodes in the start check list to all the nodes in the set connected to the goal
                    if (goalConnectedNodes.find(startSideCheckList.front().currentState) != goalConnectedNodes.end()) {
                        // find a solution path!
                        solutionFound = true;
                        /* if there is no solution found previously, or this new solution has less steps,
                         * store it in the vector 'solutionSteps' as the current solution
                         */
                        if (solutionSteps.size() == 0 || (solutionSteps.size() > startSideCheckList.front().currentPath.size() +  goalConnectedNodes[startSideCheckList.front().currentState].size())) {
                            solutionSteps.clear();
                            // copy the first part of the solution path
                            std::copy(startSideCheckList.front().currentPath.begin(), startSideCheckList.front().currentPath.end(), back_inserter(solutionSteps));
                            // the solution steps to the goal is stored in reversed order. flip it first.
                            std::reverse(goalConnectedNodes[startSideCheckList.front().currentState].begin(), goalConnectedNodes[startSideCheckList.front().currentState].end());
                            // copy the second part of the solution path
                           std::copy(goalConnectedNodes[startSideCheckList.front().currentState].begin(), goalConnectedNodes[startSideCheckList.front().currentState].end(), back_inserter(solutionSteps));
                        }
                    } else {
                        // if not find a solution path, put it in the start connected set
                        startConnectedNodes.insert(std::pair<std::string, std::vector<Moves>>(startSideCheckList.front().currentState,  startSideCheckList.front().currentPath) );
                        // put it in the start side waiting expanded list as well
                        startSideWaitList.push(startSideCheckList.front());
                    }
                    startSideCheckList.pop();
                }
                // check goal side
                while (!goalSideCheckList.empty()) {
                    expandedNodes++;
                    // compare each nodes in the goal check list to all the nodes in the set connected to the start
                    if (startConnectedNodes.find(goalSideCheckList.front().currentState) != startConnectedNodes.end()) {
                        // find a solution path!
                        solutionFound = true;
                        /* if there is no solution found previously, or this new solution has less steps,
                         * store it in the vector 'solutionSteps' as the current solution
                         */
                        if (solutionSteps.size() == 0 || (solutionSteps.size() > goalSideCheckList.front().currentPath.size() +  startConnectedNodes[goalSideCheckList.front().currentState].size())) {
                            solutionSteps.clear();
                            // copy the first part of the solution path
                            std::copy(startConnectedNodes[goalSideCheckList.front().currentState].begin(), startConnectedNodes[goalSideCheckList.front().currentState].end(), back_inserter(solutionSteps));
                            // the solution steps to the goal is stored in reversed order. flip it first.
                            std::reverse(goalSideCheckList.front().currentPath.begin(), goalSideCheckList.front().currentPath.end());
                            // copy the second part of the solution path
                            std::copy(goalSideCheckList.front().currentPath.begin(), goalSideCheckList.front().currentPath.end(), back_inserter(solutionSteps));
                        }
                    } else {
                        // if not find a solution path, put it in the start connected set
                        goalConnectedNodes.insert(std::pair<std::string, std::vector<Moves>>(goalSideCheckList.front().currentState,  goalSideCheckList.front().currentPath) );
                        // put it in the start side waiting expanded list as well
                        goalSideWaitList.push(goalSideCheckList.front());
                    }
                    goalSideCheckList.pop();
                }
                // expand start side, all the new nodes would be put in the start checking list
                while (!solutionFound && !startSideWaitList.empty()) {
                    int spacePosition = checkSpacePosition(startSideWaitList.front().currentState);
                    if (spacePosition >= ROW_SIZE) {
                        // if the puzzle state can go 'up', move up and put it in the checklist. Similiar below.
                        Nodes newNode{startSideWaitList.front().currentState, startSideWaitList.front().currentPath, 0};
                        puzzleMove(newNode.currentState, UP);
                        if (startConnectedNodes.find(newNode.currentState) == startConnectedNodes.end()) {
                            // if the new node is not already connected, add it.
                            newNode.currentPath.push_back(UP);
                            startSideCheckList.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        Nodes newNode{startSideWaitList.front().currentState, startSideWaitList.front().currentPath, 0};
                        puzzleMove(newNode.currentState, DOWN);
                        if (startConnectedNodes.find(newNode.currentState) == startConnectedNodes.end()) {
                            newNode.currentPath.push_back(DOWN);
                            startSideCheckList.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        Nodes newNode{startSideWaitList.front().currentState, startSideWaitList.front().currentPath, 0};
                        puzzleMove(newNode.currentState, LEFT);
                        if (startConnectedNodes.find(newNode.currentState) == startConnectedNodes.end()) {
                            newNode.currentPath.push_back(LEFT);
                            startSideCheckList.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        Nodes newNode{startSideWaitList.front().currentState, startSideWaitList.front().currentPath, 0};
                        puzzleMove(newNode.currentState, RIGHT);
                        if (startConnectedNodes.find(newNode.currentState) == startConnectedNodes.end()) {
                            newNode.currentPath.push_back(RIGHT);
                            startSideCheckList.push(newNode);
                            createdNodes++;
                        }
                    }
                    startSideWaitList.pop();
                }
                // expand goal side, all the new nodes would be put in the goal checking list
                while (!solutionFound && !goalSideWaitList.empty()) {
                    int spacePosition = checkSpacePosition(goalSideWaitList.front().currentState);
                    if (spacePosition >= ROW_SIZE) {
                        // if the puzzle state can go 'up', move up and put it in the checklist. Similiar below.
                        Nodes newNode{goalSideWaitList.front().currentState, goalSideWaitList.front().currentPath, 0};
                        puzzleMove(newNode.currentState, UP);
                        if (goalConnectedNodes.find(newNode.currentState) == goalConnectedNodes.end()) {
                            // if the new node is not already connected, add it.
                            newNode.currentPath.push_back(DOWN);  // notice the direction would be the opposite way
                            goalSideCheckList.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        Nodes newNode{goalSideWaitList.front().currentState, goalSideWaitList.front().currentPath, 0};
                        puzzleMove(newNode.currentState, DOWN);
                        if (goalConnectedNodes.find(newNode.currentState) == goalConnectedNodes.end()) {
                            newNode.currentPath.push_back(UP);
                            goalSideCheckList.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        Nodes newNode{goalSideWaitList.front().currentState, goalSideWaitList.front().currentPath, 0};
                        puzzleMove(newNode.currentState, LEFT);
                        if (goalConnectedNodes.find(newNode.currentState) == goalConnectedNodes.end()) {
                            newNode.currentPath.push_back(RIGHT);
                            goalSideCheckList.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        Nodes newNode{goalSideWaitList.front().currentState, goalSideWaitList.front().currentPath, 0};
                        puzzleMove(newNode.currentState, RIGHT);
                        if (goalConnectedNodes.find(newNode.currentState) == goalConnectedNodes.end()) {
                            newNode.currentPath.push_back(LEFT);
                            goalSideCheckList.push(newNode);
                            createdNodes++;
                        }
                    }
                    goalSideWaitList.pop();
                }
            }
        }
        end = std::clock();
        solveTime = (end - start) / CLOCKS_PER_MS;
        printSolution("bidirectional_search");
    }
    
    /**
     * a Manhattan distance heuristic function.
     * return an integer of total steps of each tile moving to the right place
     */
    static int heuisticManhattan(std::string state) {
        int score = 0;
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            switch (state[i]) {
                case '1':
                // i representing the position of '1'-tile, the right position should be 0
                score += abs(i % ROW_SIZE) + abs(i / ROW_SIZE);
                break;
                case '2':
                // i representing the position of '2'-tile, the right position should be 1
                score += abs(i % ROW_SIZE - 1) + abs(i / ROW_SIZE);
                break;
                case '3':
                // i representing the position of '3'-tile, the right position should be 2
                score += abs(i % ROW_SIZE - 2) + abs(i / ROW_SIZE);
                break;
                case '4':
                // i representing the position of '4'-tile, the right position should be 3
                score += abs(i % ROW_SIZE) + abs(i / ROW_SIZE - 1);
                break;
                case '5':
                // i representing the position of '5'-tile, the right position should be 4
                score += abs(i % ROW_SIZE - 1) + abs(i / ROW_SIZE - 1);
                break;
                case '6':
                // i representing the position of '3'-tile, the right position should be 2
                score += abs(i % ROW_SIZE - 2) + abs(i / ROW_SIZE - 1);
                break;
                case '7':
                // i representing the position of '1'-tile, the right position should be 0
                score += abs(i % ROW_SIZE) + abs(i / ROW_SIZE - 2);
                break;
                case '8':
                // i representing the position of '2'-tile, the right position should be 1
                score += abs(i % ROW_SIZE - 1) + abs(i / ROW_SIZE - 2);
                break;
            }
        }
        return score;
    }

    /**
     * a recursive called function for Greedy Search based on the Manhanttan distance heuristic function.
     * go to the best guess first, if it doesn't work, go to the second
     * return ture if found a solution in this movement
     */
    bool greedySearch(std::string state) {
        if (state.compare(PUZZLE_GOAL) == 0) {
            return true;
        } else {
            expandedNodes++;
            // priority node queue, smaller dirstance node will pop first
            std::priority_queue<Nodes, std::vector<Nodes>, checkByDistance> priorQueue;
            visitedStates.insert(std::pair<std::string, unsigned>(state, 0));
            // check all direction and put them in a priority of Manhattan heuistic
            int spacePosition = checkSpacePosition(state);
            std::vector<Moves> currentPath;
            std::string currentState;
            if (spacePosition >= ROW_SIZE) {
                // if the puzzle state can go 'up', move up and check if it's visited. Similiar below.
                currentState = state;
                puzzleMove(currentState, UP);
                if (visitedStates.find(currentState) == visitedStates.end()) {
                    // if the new node is not already visited, add it.
                    Nodes newNode{currentState, currentPath, heuisticManhattan(currentState)};
                    newNode.currentPath.push_back(UP);
                    priorQueue.push(newNode);
                    createdNodes++;
                }
            }
            if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                currentState = state;
                puzzleMove(currentState, DOWN);
                if (visitedStates.find(currentState) == visitedStates.end()) {
                    // if the new node is not already visited, add it.
                    Nodes newNode{currentState, currentPath, heuisticManhattan(currentState)};
                    newNode.currentPath.push_back(DOWN);
                    priorQueue.push(newNode);
                    createdNodes++;
                }
            }
            if (spacePosition % ROW_SIZE != 0) {
                currentState = state;
                puzzleMove(currentState, LEFT);
                if (visitedStates.find(currentState) == visitedStates.end()) {
                    // if the new node is not already visited, add it.
                    Nodes newNode{currentState, currentPath, heuisticManhattan(currentState)};
                    newNode.currentPath.push_back(LEFT);
                    priorQueue.push(newNode);
                    createdNodes++;
                }
            }
            if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                currentState = state;
                puzzleMove(currentState, RIGHT);
                if (visitedStates.find(currentState) == visitedStates.end()) {
                    // if the new node is not already visited, add it.
                    Nodes newNode{currentState, currentPath, heuisticManhattan(currentState)};
                    newNode.currentPath.push_back(RIGHT);
                    priorQueue.push(newNode);
                    createdNodes++;
                }
            }
            while (!solutionFound && !priorQueue.empty()) {
                if (greedySearch(priorQueue.top().currentState)) {
                    // found a solution here!
                    solutionFound = true;
                    solutionSteps.push_back(priorQueue.top().currentPath[0]);
                }
                priorQueue.pop();
            }
            return solutionFound;
        }
    }

    /**
     * conduct Greedy Search to the puzzle start state, store the result the vector 'solutionSteps'.
     * based on Manhattan heuistic function to calculate the best node first to be expanded.
     * the vector would be empty if no solution was found.
     * print out the result to a txt file at the end.
     */
    void greedy() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        visitedStates.clear();
        solutionFound = false;
        // priority node queue, smaller dirstance node will pop first
        std::priority_queue<Nodes, std::vector<Nodes>, checkByDistance> priorQueue;
        start = std::clock();
        greedySearch(startState);
        end = std::clock();
        solveTime = (end - start) / CLOCKS_PER_MS;
        // since it is recursive process, the solution step is in reversed order stored. flip it first.
        std::reverse(solutionSteps.begin(),solutionSteps.end());
        printSolution("greedy");
    }
    
    /**
     * conduct A* Search to the puzzle start state, store the result the vector 'solutionSteps'.
     * It can work with different heuistic functions, pass the function reference as a parameter.
     * Also need to provide a part of output file name as a parameter.
     * the vector would be empty if no solution was found.
     * print out the result to a txt file at the end.
     */
    void AStarSearch(int (*heuistic)(std::string), std::string outputName) {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        visitedStates.clear();
        solutionSteps.clear();
        solutionFound = false;
        // priority node queue, smaller dirstance node will pop first
        std::priority_queue<Nodes, std::vector<Nodes>, checkByDistance> priorQueue;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            visitedStates.insert(std::pair<std::string, unsigned>(startState, 0));
            // check all direction and put them in a priority of a specific heuistic function
            std::vector<Moves> startPath;
            // distance: f(n) = g(n) + h(n), here g(n) = 0 since it is the start node
            Nodes startNode{startState, startPath, 0 + (*heuistic)(startState)};
            visitedStates.insert(std::pair<std::string, unsigned>(startState, 0));
            priorQueue.push(startNode);
            createdNodes++;
            while (!solutionFound && !priorQueue.empty()) {
                if (priorQueue.top().currentState.compare(PUZZLE_GOAL) == 0) {
                    // found a solution here!
                    solutionFound = true;
                    solutionSteps = priorQueue.top().currentPath;
                } else {
                    // expand the current node
                    expandedNodes++;
                    std::string currentState = priorQueue.top().currentState;
                    std::vector<Moves> currentPath = priorQueue.top().currentPath;
                    priorQueue.pop();
                    int spacePosition = checkSpacePosition(currentState);
                    if (spacePosition >= ROW_SIZE) {
                        // if the puzzle state can go 'up', move up and check if it's visited. Similiar below.
                        std::string newState = currentState;
                        std::vector<Moves> newPath = currentPath;
                        puzzleMove(newState, UP);
                        newPath.push_back(UP);
                        if (visitedStates.find(newState) == visitedStates.end() || visitedStates[newState] > newPath.size()) {
                            // if the new node is not already visited, add it to the queue.
                            // distance: f(n) = g(n) + h(n), g(n) = the length of path to this node
                            Nodes newNode{newState, newPath, (int)newPath.size() + (*heuistic)(newState)};
                            visitedStates.insert(std::pair<std::string, unsigned>(newState, newPath.size()));
                            priorQueue.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        std::string newState = currentState;
                        std::vector<Moves> newPath = currentPath;
                        puzzleMove(newState, DOWN);
                        newPath.push_back(DOWN);
                        if (visitedStates.find(newState) == visitedStates.end() || visitedStates[newState] > newPath.size()) {
                            // if the new node is not already visited, add it to the queue.
                            // distance: f(n) = g(n) + h(n), g(n) = the length of path to this node
                            Nodes newNode{newState, newPath, (int)newPath.size() + (*heuistic)(newState)};
                            visitedStates.insert(std::pair<std::string, unsigned>(newState, newPath.size()));
                            priorQueue.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        std::string newState = currentState;
                        std::vector<Moves> newPath = currentPath;
                        puzzleMove(newState, LEFT);
                        newPath.push_back(LEFT);
                        if (visitedStates.find(newState) == visitedStates.end() || visitedStates[newState] > newPath.size()) {
                            // if the new node is not already visited, add it to the queue.
                            // distance: f(n) = g(n) + h(n), g(n) = the length of path to this node
                            Nodes newNode{newState, newPath, (int)newPath.size() + (*heuistic)(newState)};
                            visitedStates.insert(std::pair<std::string, unsigned>(newState, newPath.size()));
                            priorQueue.push(newNode);
                            createdNodes++;
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        std::string newState = currentState;
                        std::vector<Moves> newPath = currentPath;
                        puzzleMove(newState, RIGHT);
                        newPath.push_back(RIGHT);
                        if (visitedStates.find(newState) == visitedStates.end() || visitedStates[newState] > newPath.size()) {
                            // if the new node is not already visited, add it to the queue.
                            // distance: f(n) = g(n) + h(n), g(n) = the length of path to this node
                            Nodes newNode{newState, newPath, (int)newPath.size() + (*heuistic)(newState)};
                            visitedStates.insert(std::pair<std::string, unsigned>(newState, newPath.size()));
                            priorQueue.push(newNode);
                            createdNodes++;
                        }
                    }
                }
            }
        }
        end = std::clock();
        solveTime = (end - start) / CLOCKS_PER_MS;
        printSolution(outputName);
    }
    
};

#endif /* EightPuzzle_h */
