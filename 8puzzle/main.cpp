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
//        newPuzzle.breadthFirst();
//        newPuzzle.depthFirst();
//        newPuzzle.depthLimited();
//        newPuzzle.iterativeDeepening();
//        newPuzzle.bidirectionalSearch();
//        newPuzzle.greedy();
//        newPuzzle.AStarSearch(EightPuzzle::heuristicManhattan, "a_star_manhattan");
        newPuzzle.AStarSearch(EightPuzzle::heuristicRefine, "a_star_refine");
    }
    
    return 0;
}
