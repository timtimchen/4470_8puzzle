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
#include <set>
#include <time.h>
#include <algorithm>    // std::reverse

#define CLOCKS_PER_MS (clock_t(1))
#define PUZZLE_SIZE 9
#define ROW_SIZE 3
#define MAX_STEPS 40

const std::string PUZZLE_GOAL = "12345678_";

enum Moves { UP, DOWN, LEFT, RIGHT };

class EightPuzzle {
    private:
    std::string startState;
    bool solutionFound;
    std::vector<Moves> solutionSteps;
    std::set<std::string> visitedStates;
    int createdNodes;
    int expandedNodes;
    int solveTime;  //in milliseconds

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
    
    bool depthFirstSearch(std::string state, Moves direction, int depth) {
        if (depth > MAX_STEPS) {
            return false;
        } else {
            puzzleMove(state, direction);
            if (state.compare(PUZZLE_GOAL) == 0) {
                return true;
            } else if (visitedStates.find(state) != visitedStates.end()) {
                return false;
            } else {
                visitedStates.insert(state);
                int spacePosition = checkSpacePosition(state);
                if ((spacePosition % ROW_SIZE != ROW_SIZE - 1) && depthFirstSearch(state, RIGHT, depth + 1)) {
                    solutionSteps.push_back(RIGHT);
                    return true;
                } else if ((spacePosition < PUZZLE_SIZE - ROW_SIZE) && depthFirstSearch(state, DOWN, depth + 1)) {
                    solutionSteps.push_back(DOWN);
                    return true;
                } else if ((spacePosition % ROW_SIZE != 0) && depthFirstSearch(state, LEFT, depth + 1)) {
                    solutionSteps.push_back(LEFT);
                    return true;
                } else if (spacePosition >= ROW_SIZE && depthFirstSearch(state, UP, depth + 1)) {
                    solutionSteps.push_back(UP);
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    void depthFirst() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        visitedStates.clear();
        solutionFound = true;
        if (startState.compare(PUZZLE_GOAL) != 0) {
            visitedStates.insert(startState);
            int spacePosition = checkSpacePosition(startState);
            if ((spacePosition % ROW_SIZE != ROW_SIZE - 1) && depthFirstSearch(startState, RIGHT, 1)) {
                solutionSteps.push_back(RIGHT);
            } else if ((spacePosition < PUZZLE_SIZE - ROW_SIZE) && depthFirstSearch(startState, DOWN, 1)) {
                solutionSteps.push_back(DOWN);
            } else if ((spacePosition % ROW_SIZE != 0) && depthFirstSearch(startState, LEFT, 1)) {
                solutionSteps.push_back(LEFT);
            } else if (spacePosition >= ROW_SIZE && depthFirstSearch(startState, UP, 1)) {
                solutionSteps.push_back(UP);
            } else {
                solutionFound = false;
            }
        }
        std::reverse(solutionSteps.begin(),solutionSteps.end());
        printSolution("depth_first");
    }
};

#endif /* EightPuzzle_h */
