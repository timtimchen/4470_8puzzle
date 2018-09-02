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
#include <vector>
#include <time.h>

#define CLOCKS_PER_MS (clock_t(1))
#define PUZZLE_SIZE 9

const char PUZZLE_GOAL[PUZZLE_SIZE] = {'1', '2', '3', '4', '5', '6', '7', '8', '_'};
const int MAX_STEPS = 20;

enum Moves { UP, DOWN, LEFT, RIGHT };

class EightPuzzle {
    private:
    char startState[PUZZLE_SIZE];
    
    public:
    EightPuzzle() {
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            startState[i] = PUZZLE_GOAL[i];
        }
    }
    
    void printSolution(std::string searchName, const std::vector<Moves>& steps) {
        char currentState[PUZZLE_SIZE];
        for (int i = 0; i < PUZZLE_SIZE; i++) {
            currentState[i] = startState[i];
        }
        std::ofstream myfile;
        myfile.open (searchName + "_result.txt");
        myfile << "Writing this to a file.\n";
        myfile.close();
        
    }
    
};

#endif /* EightPuzzle_h */
