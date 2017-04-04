#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "ACO.h"

using namespace std;

void printInfo() {
    cout << endl;
    cout << "Usage:\nFor all Ant Colony Optimization:\n./main filename" << endl;
    cout << "filename = path to the tsp file to use" << endl;
    cout << "Sample input: './main ALL_tsp/att48.tsp'" << endl;
    cout << endl;
}


int main (int argc, char** argv) {
    if(argc != 2) {
        // incorrect input
        printInfo();
        exit(1);
    } else {
        string fileName = argv[1];
        cout << fileName << endl;

        ACOSolver solver(fileName);
    }
}
