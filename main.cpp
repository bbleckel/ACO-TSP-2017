#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "ACO.h"



void printInfo() {
	cout << endl;
	cout << "Usage:\nFor Elite Ant System:\n./main algorithm numAnts iterations pheromoneInfluence heuristicInfluence evaporation elitism" << endl;
	cout << "    algorithm           = type of algorithm, Elite Ant System or Ant Colony System (eas or acs) (string)" << endl;
  cout << "    numAnts             = number of ants in the colony (int)" << endl;
	cout << "    iterations          = number of iterations (int)" << endl;
	cout << "    pheromoneInfluence  = alpha, the degree of influence of the pheromone component (double):" << endl;
  cout << "    heuristicInfluence  = beta, the degree of influence of the heuristic component (double):" << endl;
  cout << "    evaporation         = rho, the pheromone evaporation factor (double):" << endl;
  cout << "    elitism             = e, the elitism factor (double):" << endl;
  cout << "Usage:\nFor Ant Colony System:\n./main algorithm numAnts iterations pheromoneInfluence heuristicInfluence evaporation epsilon tau0 q0" << endl;
  cout << "    algorithm           = type of algorithm, Elite Ant System or Ant Colony System (eas or acs) (string)" << endl;
	cout << "    numAnts             = number of ants in the colony (int)" << endl;
	cout << "    iterations          = number of iterations (int)" << endl;
	cout << "    pheromoneInfluence  = alpha, the degree of influence of the pheromone component (double):" << endl;
  cout << "    heuristicInfluence  = beta, the degree of influence of the heuristic component (double):" << endl;
  cout << "    evaporation         = rho, the pheromone evaporation factor (double):" << endl;
  cout << "    epsilon             = factor controlling wearing away of pheromones (double):" << endl;
  cout << "    tau0                = factor controlling wearing away of pheromones (double):" << endl;
  cout << "    q0                  = probability that an ant will choose the best leg for the next leg of the tour it is constructing, instead of choosing probabilistically (double)" << endl;
	cout << endl;
}


int main (int argc, char** argv) {
		int algtype; // 0 for EAS, 1 for ACS
		int numAnts;
		int iterations;
		double pheroInfluence;
		double heurInfluence;
		double evaporation;

		// this one only for EAS
		double elitism;

		// these three only for ACS
		double epsilon;
		double tau0;
		double q0;

		if(argc != 8 && argc != 10) {
			// incorrect input
			printInfo();
			exit(1);
		} else {
			// type of algorithm determines how other arguments are interpreted
			if (!strcmp(argv[1], "eas")) {
				algType = 0;
			} else if (!strcmp(argv[1], "acs")) {
				algType = 2;
			} else {
				cout << "Invalid first argument specifying algorithm type. Please use:" << endl;
				cout << "    algorithm    = type of algorithm, Elite Ant System or Ant Colony System (eas or acs) (string)" << endl;
        exit(1);
			}
		}



}
