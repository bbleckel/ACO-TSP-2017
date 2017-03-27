#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "ACO.h"



void printInfo() {
	cout << endl;
	cout << "Usage:\nFor Elite Ant System:\n./main numAnts iterations pheromoneInfluence heuristicInfluence evaporation elitism" << endl;
	cout << "    numAnts             = number of ants in the colony (int)" << endl;
	cout << "    iterations          = number of iterations (int)" << endl;
	cout << "    pheromoneInfluence  = alpha, the degree of influence of the pheromone component (double):" << endl;
  cout << "    heuristicInfluence  = beta, the degree of influence of the heuristic component (double):" << endl;
  cout << "    evaporation         = rho, the pheromone evaporation factor (double):" << endl;
  cout << "    elitism             = e, the elitism factor (double):" << endl;
  cout << "Usage:\nFor Ant Colony System:\n./main numAnts iterations pheromoneInfluence heuristicInfluence evaporation epsilon tau0 q0" << endl;
	cout << "    numAnts             = number of ants in the colony (int)" << endl;
	cout << "    iterations          = number of iterations (int)" << endl;
	cout << "    pheromoneInfluence  = alpha, the degree of influence of the pheromone component (double):" << endl;
  cout << "    heuristicInfluence  = beta, the degree of influence of the heuristic component (double):" << endl;
  cout << "    evaporation         = rho, the pheromone evaporation factor (double):" << endl;
  cout << "    epsilon             = factor controlling wearing away of pheromones (double):" << endl;
  cout << "    tau0                = factor controlling wearing away of pheromones (double):" << endl;
  cout << "    q0                  = the probability in Ant Colony System that an ant will choose the best leg for the next leg of the tour it is constructing, instead of choosing probabilistically (double):" << endl;
	cout << endl;
}


int main (int argc, char** argv) {
  



}
