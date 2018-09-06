//
//  main.cpp
//  8puzzle
//
//  Created by jing hong chen on 9/2/18.
//  Copyright © 2018 jing hong chen. All rights reserved.
//

#include "EightPuzzle.h"
#include <stdlib.h>     /* srand, rand */

using namespace std;

int main(int argc, const char * argv[]) {
    EightPuzzle newPuzzle;
    if (argc < 2) {
        cout << "Please input the puzzle cofing file name in the command line." << endl;
    }
    else {
        newPuzzle.puzzleInit(argv[1]);
        newPuzzle.breadthFirst();
//        newPuzzle.depthFirst();
//        newPuzzle.depthLimited();
//        newPuzzle.iterativeDeepening();
//        newPuzzle.bidirectionalSearch();
//        newPuzzle.greedy();
//        newPuzzle.AStarSearch(EightPuzzle::heuisticManhattan, "a_star_manhattan");
//        newPuzzle.AStarSearch(EightPuzzle::SmartHeuistic, "a_star_smart");
    }
    
    return 0;
}
