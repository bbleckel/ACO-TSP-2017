#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "ACO.h"

using namespace std;

void printMedian(vector<vector<double> > trials, int num);

void printInfo() {
    cout << endl;
    cout << "Usage:\nFor all Ant Colony Optimization:\n./main filename" << endl;
    cout << "filename = path to the tsp file to use" << endl;
    cout << "Sample input: './main ALL_tsp/att48.tsp'" << endl;
    cout << endl;
}

string files[4] = {"d2103","u2311","u2152","pr2392"};
double alphas[5] = {0.5,1.0,1.2,1.4,1.6};
double betas[5] = {1,2,3,4,5};
double rhos[5] = {0.01, 0.1, 0.25, 0.5, 0.75};
double epsilons[5] = {0.01, 0.1, 0.25, 0.5, 0.75};
double qs[6] = {0.1, 0.25, 0.5, 0.75, 0.9, 1};
const static double TRIALS = 1;

bool compare(vector<double > i, vector<double> j) {
    return (i[i.size() - 2], j[j.size() - 2]);
}

void test() {
    int num = 0;
    // for(int q = 0; q < 6; q++) {
        for(int eps = 3; eps < 5; eps++) {
            // for(int f = 0; f < 4; f++) {
                // for(int a = 0; a < 5; a++) {
                    // for(int b = 0; b < 5; b++) {
                        // for(int r = 0; r < 5; r++) {
                        int f = 0;
                        int a = 2;
                        int b = 4;
                        int r = 1;
                        int q = 4;
                        // int eps = 1;
                            vector<vector<double> > trials;
                            for(int i = 0; i < TRIALS; i++) {
                                string fileName = "ALL_tsp/" + files[f] + ".tsp";
                                ACOSolver solver(fileName, qs[q], epsilons[eps], alphas[a], betas[b], rhos[r]);

                                trials.push_back(solver.solve());
                            }
                            sort(trials.begin(), trials.end(), compare);
                            cout << "% **INPUTS** file: " << files[f] << ", alpha: " << alphas[a];
                            cout << ", beta: " << betas[b] << ", rho: " << rhos[r] << ", epsilon: " << epsilons[eps];
                            cout << "; median best over " << TRIALS << " trials." << endl;
                            cout << "% The best from file is: ";
                            cout << trials[0][trials[0].size() - 1] << endl;
                            printMedian(trials, num);
                            num++;
                        // }
                    // }
                // }
            // }
        }
    // }
}

void printMedian(vector<vector<double> > trials, int num) {
    int med = trials.size()/2;

    cout << "\\begin{filecontents*}{data" << num << ".txt}" << endl;
    //only go to size() - 1 (i.e. exclude the last one) because the last one is the optimal
    for(int i = 0; i < trials[med].size() - 1; i++) {
        cout << "\t" << i << "  " << trials[med][i] << endl;
    }
    cout << "\\end{filecontents*}" << endl;
    cout << endl << endl;
}

int main (int argc, char** argv) {
    if(argc != 2) {
        // incorrect input
        printInfo();
        exit(1);
    } else {
        // string fileName = argv[1];
        // cout << fileName << endl;
        //
        // ACOSolver solver(fileName);
        //
        // solver.solve();

        test();
    }

}
