//
//  main.cpp
//  8puzzle
//
//  Created by jing hong chen on 9/2/18.
//  Copyright © 2018 jing hong chen. All rights reserved.
//

#include "EightPuzzle.h"

using namespace std;

int main(int argc, const char * argv[]) {
    vector<Moves> solutionSteps;
    
    EightPuzzle newPuzzle;
    newPuzzle.printSolution("test", solutionSteps);
    
    return 0;
}
