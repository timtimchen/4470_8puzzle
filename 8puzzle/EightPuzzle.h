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
#include <time.h>

#define CLOCKS_PER_MS (clock_t(1))
#define PUZZLE_SIZE 9
#define ROW_SIZE 3
#define MAX_STEPS 20

const char PUZZLE_GOAL[PUZZLE_SIZE] = {
    '1', '2', '3',
    '4', '5', '6',
    '7', '8', '_'
};

enum Moves { UP, DOWN, LEFT, RIGHT };

class EightPuzzle {
    private:
    char startState[PUZZLE_SIZE];
    std::vector<Moves> solutionSteps;
    int createdNodes;
    int expandedNodes;
    int solveTime;  //in milliseconds

    public:
    EightPuzzle() {
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            startState[i] = PUZZLE_GOAL[i];
        }
    }
    
    void storeStringToPuzzle(char* puzzleState, std::string input) {
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            puzzleState[i] = input.at(i);
        }
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
                storeStringToPuzzle(startState, inputStr);
            } else {
                std::cout << "The file " << fileName << " doesn't contain a correct puzzle state." << std::endl;
            }
        } else {
            std::cout << "Cannot open the file " << fileName << std::endl;
        }
    }
    
    std::string checkSolutionSteps(char* state, Moves direction) {
        std::string moveResult = "";
        int spacePosition = -1;
        int switchPosstion = -1;
        char tempChar;
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            if (state[i] == '_') {
                spacePosition = i;
            }
        }
        switch (direction) {
            case UP:
            if (spacePosition < ROW_SIZE) {
                moveResult = "STAY ";
            } else {
                moveResult = " UP  ";
                switchPosstion = spacePosition - ROW_SIZE;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case DOWN:
            if (spacePosition >= PUZZLE_SIZE - ROW_SIZE) {
                moveResult = "STAY ";
            } else {
                moveResult = "DOWN ";
                switchPosstion = spacePosition + ROW_SIZE;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case LEFT:
            if ((spacePosition % ROW_SIZE) == 0) {
                moveResult = "STAY ";
            } else {
                moveResult = "LEFT ";
                switchPosstion = spacePosition - 1;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
            case RIGHT:
            if ((spacePosition % ROW_SIZE) == ROW_SIZE - 1) {
                moveResult = "STAY ";
            } else {
                moveResult = "RIGHT";
                switchPosstion = spacePosition + 1;
                tempChar = state[switchPosstion];
                state[switchPosstion] = state[spacePosition];
                state[spacePosition] = tempChar;
            }
            break;
        }
        return moveResult;
    }
    
    void printSolution(std::string searchName) {
        char currentState[PUZZLE_SIZE];
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            currentState[i] = startState[i];
        }
        std::ofstream myfile;
        myfile.open (searchName + "_result.txt");
        myfile << "                 " << currentState[0] << currentState[1] << currentState[2] << std::endl;
        myfile << "Starting puzzle: " << currentState[3] << currentState[4] << currentState[5] << std::endl;
        myfile << "                 " << currentState[6] << currentState[7] << currentState[8] << std::endl;
        std::string movement;
        for (int i = 0; i < solutionSteps.size(); i++) {
            movement = checkSolutionSteps(currentState, solutionSteps[i]);
            myfile << std::endl;
            myfile << "                 " << currentState[0] << currentState[1] << currentState[2] << std::endl;
            myfile << " --- " << movement <<" --->  " << currentState[3] << currentState[4] << currentState[5] << std::endl;
            myfile << "                 " << currentState[6] << currentState[7] << currentState[8] << std::endl;
        }
        myfile.close();
    }
    
    void depthFirst() {
        createdNodes = 0;
        expandedNodes = 0;
        solveTime = 0;
        solutionSteps.clear();
        printSolution("depth_first");
    }
};

#endif /* EightPuzzle_h */
