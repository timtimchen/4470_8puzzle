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
#include <algorithm>    // std::reverse

#define CLOCKS_PER_MS (clock_t(1))
#define PUZZLE_SIZE 9
#define ROW_SIZE 3
#define MAX_STEPS 50

const std::string PUZZLE_GOAL = "12345678_";

enum Moves { UP, DOWN, LEFT, RIGHT };

struct Nodes {
    std::string currentState;
    std::vector<Moves> currentPath;
};

class EightPuzzle {
    private:
    std::string startState;
    bool solutionFound;
    std::vector<Moves> solutionSteps;
    std::map<std::string, int> visitedStates;  // record visited node state and the current cost to reach it
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
    
    int checkSpacePosition(std::string state) {
        int position = -1;
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            if (state[i] == '_') {
                position = i;
            }
        }
        return position;
    }
    
    std::string checkSolutionSteps(std::string& state, Moves direction) {
        std::string movementResult = "";
        int switchPosstion = -1;
        char tempChar;
        int spacePosition = checkSpacePosition(state);
        switch (direction) {
            case UP:
            if (spacePosition < ROW_SIZE) {
                movementResult = "STAY ";
            } else {
                movementResult = " UP  ";
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
    
    void puzzleMove(std::string& state, Moves dir) {
        int switchPosstion = -1;
        char tempChar;
        int spacePosition = checkSpacePosition(state);
        switch (dir) {
            case UP:
            if (spacePosition >= ROW_SIZE) {
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
    
    void printSolution(std::string searchName) {
        std::string currentState = startState;
        std::ofstream myfile;
        myfile.open (searchName + "_result.txt");
        myfile << "search strategy: " << searchName << std::endl << std::endl;
        myfile << "Running time (in milliseconds): "  << solveTime << std::endl << std::endl;
        myfile << "Expanded Nodes: "  << visitedStates.size() << std::endl << std::endl;
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
    
    void breadthFirst() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        visitedStates.clear();
        solutionFound = false;
        std::queue<Nodes> nodeQueue;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            std::vector<Moves> path;
            Nodes startNode{startState, path};
            nodeQueue.push(startNode);
            while (!solutionFound && !nodeQueue.empty()) {
                if (nodeQueue.front().currentState.compare(PUZZLE_GOAL) == 0) {
                    solutionFound = true;
                    solutionSteps = nodeQueue.front().currentPath;
                } else {
                    // insert current node to vistied map, current cost is always minimum so being ignored in BFS
                    visitedStates.insert(std::pair<std::string, int>(nodeQueue.front().currentState, 0));
                    int spacePosition = checkSpacePosition(nodeQueue.front().currentState);
                    if (spacePosition >= ROW_SIZE) {
                        Nodes newNode{nodeQueue.front().currentState, nodeQueue.front().currentPath};
                        puzzleMove(newNode.currentState, UP);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(UP);
                            nodeQueue.push(newNode);
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        Nodes newNode{nodeQueue.front().currentState, nodeQueue.front().currentPath};
                        puzzleMove(newNode.currentState, DOWN);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(DOWN);
                            nodeQueue.push(newNode);
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        Nodes newNode{nodeQueue.front().currentState, nodeQueue.front().currentPath};
                        puzzleMove(newNode.currentState, LEFT);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(LEFT);
                            nodeQueue.push(newNode);
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        Nodes newNode{nodeQueue.front().currentState, nodeQueue.front().currentPath};
                        puzzleMove(newNode.currentState, RIGHT);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(RIGHT);
                            nodeQueue.push(newNode);
                        }
                    }
                    nodeQueue.pop();
                }
            }
        }
        end = std::clock();
        solveTime = (end - start) / CLOCKS_PER_MS;
        printSolution("breadth_first");
    }
    
    void depthFirst() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        visitedStates.clear();
        solutionFound = false;
        std::stack<Nodes> nodeStack;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            std::vector<Moves> path;
            Nodes startNode{startState, path};
            nodeStack.push(startNode);
            while (!solutionFound && !nodeStack.empty()) {
                if (nodeStack.top().currentState.compare(PUZZLE_GOAL) == 0) {
                    solutionFound = true;
                    solutionSteps = nodeStack.top().currentPath;
                } else {
                    std::string currentState =nodeStack.top().currentState;
                    std::vector<Moves> currentPath = nodeStack.top().currentPath;
                    nodeStack.pop();
                    visitedStates.insert(std::pair<std::string, int>(currentState, 0));
                    int spacePosition = checkSpacePosition(currentState);
                    if (spacePosition >= ROW_SIZE) {
                        Nodes newNode{currentState, currentPath};
                        puzzleMove(newNode.currentState, UP);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(UP);
                            nodeStack.push(newNode);
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        Nodes newNode{currentState, currentPath};
                        puzzleMove(newNode.currentState, DOWN);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(DOWN);
                            nodeStack.push(newNode);
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        Nodes newNode{currentState, currentPath};
                        puzzleMove(newNode.currentState, LEFT);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(LEFT);
                            nodeStack.push(newNode);
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        Nodes newNode{currentState, currentPath};
                        puzzleMove(newNode.currentState, RIGHT);
                        if (visitedStates.find(newNode.currentState) == visitedStates.end()) {
                            newNode.currentPath.push_back(RIGHT);
                            nodeStack.push(newNode);
                        }
                    }
                }
            }
        }
        end = std::clock();
        solveTime = (end - start) / CLOCKS_PER_MS;
        printSolution("depth_first");
    }

    bool depthLimitedSearch(std::string state, Moves direction, int depth) {
        if (depth > MAX_STEPS) {
            return false;
        } else {
            puzzleMove(state, direction);
            if (state.compare(PUZZLE_GOAL) == 0) {
                return true;
            } else if (visitedStates.find(state) != visitedStates.end() && visitedStates[state] <= depth) {
                return false;
            } else {
                // insert a new visited node, or update its cost if already existed
                visitedStates[state] = depth;
                int spacePosition = checkSpacePosition(state);
                if (spacePosition >= ROW_SIZE && depthLimitedSearch(state, UP, depth + 1)) {
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
    
    void depthLimited() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        visitedStates.clear();
        solutionFound = true;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            visitedStates.insert(std::pair<std::string, int>(startState, 0));
            int spacePosition = checkSpacePosition(startState);
            if (spacePosition >= ROW_SIZE && depthLimitedSearch(startState, UP, 1)) {
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
        std::reverse(solutionSteps.begin(),solutionSteps.end());
        printSolution("depth_limited");
    }

    bool iterativeDeepeningSearch(std::string state, Moves direction, int start_depth, int max_depth) {
        if (start_depth > max_depth) {
            return false;
        } else {
            puzzleMove(state, direction);
            if (state.compare(PUZZLE_GOAL) == 0) {
                return true;
            } else if (visitedStates.find(state) != visitedStates.end() && visitedStates[state] <= start_depth) {
                return false;
            } else {
                // insert a new visited node, or update its cost if already existed
                visitedStates[state] = start_depth;
                int spacePosition = checkSpacePosition(state);
                if (spacePosition >= ROW_SIZE && iterativeDeepeningSearch(state, UP, start_depth + 1, max_depth)) {
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
    
    void iterativeDeepening() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        solutionFound = false;
        int current_max_depth = 0;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            int spacePosition = checkSpacePosition(startState);
            while (!solutionFound) {
                visitedStates.clear();
                visitedStates.insert(std::pair<std::string, int>(startState, 0));
                current_max_depth++;
                if (spacePosition >= ROW_SIZE && iterativeDeepeningSearch(startState, UP, 1, current_max_depth)) {
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
        std::reverse(solutionSteps.begin(),solutionSteps.end());
        printSolution("iterative_deepening");
    }

    void bidirectionalSearch() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        solutionFound = false;
        std::queue<Nodes> startSideCheckList;
        std::queue<Nodes> goalSideCheckList;
        std::queue<Nodes> startSideWaitList;
        std::queue<Nodes> goalSideWaitList;
        std::map<std::string, std::vector<Moves>> startConnectedNodes;
        std::map<std::string, std::vector<Moves>> goalConnectedNodes;
        start = std::clock();
        if (startState.compare(PUZZLE_GOAL) != 0) {
            std::vector<Moves> path;
            Nodes startNode{startState, path};
            Nodes goalNode{PUZZLE_GOAL, path};
            startSideCheckList.push(startNode);
            goalSideCheckList.push(goalNode);
            while (!solutionFound && !(startSideCheckList.empty() && goalSideCheckList.empty())) {
                // check start side
                while (!startSideCheckList.empty()) {
                    if (goalConnectedNodes.find(startSideCheckList.front().currentState) != goalConnectedNodes.end()) {
                        // find a solution path!
                        solutionFound = true;
                        if (solutionSteps.size() == 0 || (solutionSteps.size() > startSideCheckList.front().currentPath.size() +  goalConnectedNodes[startSideCheckList.front().currentState].size())) {
                            solutionSteps.clear();
                            std::copy(startSideCheckList.front().currentPath.begin(), startSideCheckList.front().currentPath.end(), back_inserter(solutionSteps));
                            std::reverse(goalConnectedNodes[startSideCheckList.front().currentState].begin(), goalConnectedNodes[startSideCheckList.front().currentState].end());
                            std::copy(goalConnectedNodes[startSideCheckList.front().currentState].begin(), goalConnectedNodes[startSideCheckList.front().currentState].end(), back_inserter(solutionSteps));
                        }
                    } else {
                        // if not find a solution path, put it in the start connected set
                        startConnectedNodes.insert(std::pair<std::string, std::vector<Moves>>(startSideCheckList.front().currentState,  startSideCheckList.front().currentPath) );
                        startSideWaitList.push(startSideCheckList.front());
                    }
                    startSideCheckList.pop();
                }
                // check goal side
                while (!goalSideCheckList.empty()) {
                    if (startConnectedNodes.find(goalSideCheckList.front().currentState) != startConnectedNodes.end()) {
                        // find a solution path!
                        solutionFound = true;
                        if (solutionSteps.size() == 0 || (solutionSteps.size() > goalSideCheckList.front().currentPath.size() +  startConnectedNodes[goalSideCheckList.front().currentState].size())) {
                            solutionSteps.clear();
                            std::copy(startConnectedNodes[goalSideCheckList.front().currentState].begin(), startConnectedNodes[goalSideCheckList.front().currentState].end(), back_inserter(solutionSteps));
                            std::reverse(goalSideCheckList.front().currentPath.begin(), goalSideCheckList.front().currentPath.end());
                            std::copy(goalSideCheckList.front().currentPath.begin(), goalSideCheckList.front().currentPath.end(), back_inserter(solutionSteps));
                        }
                    } else {
                        // if not find a solution path, put it in the start connected set
                        goalConnectedNodes.insert(std::pair<std::string, std::vector<Moves>>(goalSideCheckList.front().currentState,  goalSideCheckList.front().currentPath) );
                        goalSideWaitList.push(goalSideCheckList.front());
                    }
                    goalSideCheckList.pop();
                }
                // expand start side
                while (!solutionFound && !startSideWaitList.empty()) {
                    int spacePosition = checkSpacePosition(startSideWaitList.front().currentState);
                    if (spacePosition >= ROW_SIZE) {
                        Nodes newNode{startSideWaitList.front().currentState, startSideWaitList.front().currentPath};
                        puzzleMove(newNode.currentState, UP);
                        if (startConnectedNodes.find(newNode.currentState) == startConnectedNodes.end()) {
                            newNode.currentPath.push_back(UP);
                            startSideCheckList.push(newNode);
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        Nodes newNode{startSideWaitList.front().currentState, startSideWaitList.front().currentPath};
                        puzzleMove(newNode.currentState, DOWN);
                        if (startConnectedNodes.find(newNode.currentState) == startConnectedNodes.end()) {
                            newNode.currentPath.push_back(DOWN);
                            startSideCheckList.push(newNode);
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        Nodes newNode{startSideWaitList.front().currentState, startSideWaitList.front().currentPath};
                        puzzleMove(newNode.currentState, LEFT);
                        if (startConnectedNodes.find(newNode.currentState) == startConnectedNodes.end()) {
                            newNode.currentPath.push_back(LEFT);
                            startSideCheckList.push(newNode);
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        Nodes newNode{startSideWaitList.front().currentState, startSideWaitList.front().currentPath};
                        puzzleMove(newNode.currentState, RIGHT);
                        if (startConnectedNodes.find(newNode.currentState) == startConnectedNodes.end()) {
                            newNode.currentPath.push_back(RIGHT);
                            startSideCheckList.push(newNode);
                        }
                    }
                    startSideWaitList.pop();
                }
                // expand goal side
                while (!solutionFound && !goalSideWaitList.empty()) {
                    int spacePosition = checkSpacePosition(goalSideWaitList.front().currentState);
                    if (spacePosition >= ROW_SIZE) {
                        Nodes newNode{goalSideWaitList.front().currentState, goalSideWaitList.front().currentPath};
                        puzzleMove(newNode.currentState, UP);
                        if (goalConnectedNodes.find(newNode.currentState) == goalConnectedNodes.end()) {
                            newNode.currentPath.push_back(DOWN);
                            goalSideCheckList.push(newNode);
                        }
                    }
                    if (spacePosition < PUZZLE_SIZE - ROW_SIZE) {
                        Nodes newNode{goalSideWaitList.front().currentState, goalSideWaitList.front().currentPath};
                        puzzleMove(newNode.currentState, DOWN);
                        if (goalConnectedNodes.find(newNode.currentState) == goalConnectedNodes.end()) {
                            newNode.currentPath.push_back(UP);
                            goalSideCheckList.push(newNode);
                        }
                    }
                    if (spacePosition % ROW_SIZE != 0) {
                        Nodes newNode{goalSideWaitList.front().currentState, goalSideWaitList.front().currentPath};
                        puzzleMove(newNode.currentState, LEFT);
                        if (goalConnectedNodes.find(newNode.currentState) == goalConnectedNodes.end()) {
                            newNode.currentPath.push_back(RIGHT);
                            goalSideCheckList.push(newNode);
                        }
                    }
                    if (spacePosition % ROW_SIZE != ROW_SIZE - 1) {
                        Nodes newNode{goalSideWaitList.front().currentState, goalSideWaitList.front().currentPath};
                        puzzleMove(newNode.currentState, RIGHT);
                        if (goalConnectedNodes.find(newNode.currentState) == goalConnectedNodes.end()) {
                            newNode.currentPath.push_back(LEFT);
                            goalSideCheckList.push(newNode);
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
    
};

#endif /* EightPuzzle_h */
