#include "ACO.h"

using namespace std;

void printPoint(point2D p) {
    cout << "(" << p.x << ", " << p.y << ")" << endl;
}

void printCity(City c) {
    cout << "City " << c.ID << ": (" << c.p.x << ", " << c.p.y << ")" << endl;
}

// calculates the distance between two cities
double ACOSolver::calculateDistance(point2D city1, point2D city2) {
    double aSquared = (city1.x - city2.x) * (city1.x - city2.x);
    double bSquared = (city1.y - city2.y) * (city1.y - city2.y);
    double cSquared = aSquared + bSquared;
    double distance = pow(cSquared, 0.5);
    return distance;
}

int getRandomCity(vector<City> unvisited) {
    int randomIndex = rand() % unvisited.size();
    return randomIndex;
}

// returns true if there is a path from city1 to city2 in the bsf path
bool ACOSolver::inBSF(City city1, City city2) {
    bool isInBSF = false;
    // check if the two cities are next to each other in the bsf list
    for(int i = 0; i < bsfRoute.size(); i++) {
      // have to loop to the back of the vector
      if (bsfRoute[i] == city1.ID) {
        if (i == 0) {
          if (bsfRoute[bsfRoute.size() - 1] == city2.ID) {
            isInBSF = true;
          }
          if (bsfRoute[1] == city2.ID) {
            isInBSF = true;
          }

          // have to loop back to the front of the vector
        } else if (i == bsfRoute.size() - 1) {
          if (bsfRoute[bsfRoute.size() - 2] == city2.ID) {
            isInBSF = true;
          }
          if (bsfRoute[0] == city2.ID) {
            isInBSF = true;
          }

          // can just check either direction
        } else {
          if (bsfRoute[i - 1] == city2.ID) {
            isInBSF = true;
          }
          if (bsfRoute[i + 1] == city2.ID) {
            isInBSF = true;
          }
        }
      }
    }

    return isInBSF;
}

// puts all possible legs in a vector
void ACOSolver::initAllLegs() {
    for (int i = 0; i < cities.size() - 1; i++) {
        for (int j = i + 1; j < cities.size(); j++) {
            Leg tempLeg;
            tempLeg.city1 = cities[i].p;
            tempLeg.city2 = cities[j].p;
            tempLeg.phero = 0.0;
            legs.push_back(tempLeg);
        }
    }
}

ACOSolver::ACOSolver(string fileName) {
    // constructor
    this->fileName = fileName;
    readFile();

    initAllLegs();
}

ACOSolver::~ACOSolver() {
    // destructor
}

void ACOSolver::readFile() {
    // read file
    string line;
    ifstream inputFile;
    inputFile.open(fileName, ios::in);

    int x, y, ID;
    point2D p;

    int started = 0;
    if(!inputFile.is_open()) {
        cerr << "ERROR: Could not open file" << endl;
        exit(1);
    } else {
        while(getline(inputFile, line)) {
            if (line.front() == '1') {
                started = 1;
                string entry;
                string delimiter = " ";

                City c;

                // store line number as city ID
                ID = stoi(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                // save coordinates
                x = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());
                y = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                p.x = x;
                p.y = y;

                c.p = p;
                c.ID = ID;

                cities.push_back(c);

            } else if (line.front() == 'E' && line.back() == 'F') {
                // EOF found -- done
                break;
            } else if (started) {
                string entry;
                string delimiter = " ";

                City c;

                // store line number as city ID
                ID = stoi(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                x = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());
                y = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                p.x = x;
                p.y = y;

                c.p = p;
                c.ID = ID;

                cities.push_back(c);
            }
        }
        inputFile.close();
    }
    cout << "Printing cities!" << endl;
    for(int i = 0; i < cities.size(); i++) {
        printCity(cities[i]);
    }
}

void ACOSolver::ACSPheroUpdate(double oldPhero) {
    // iterate through legs, updating pheromones
    for(int i = 0; i < legs.size(); i++) {
        double newPhero = oldPhero;
        double deltaTotal = 0;

        newPhero = (1 - RHO) * oldPhero + deltaTotal;
        legs[i].phero = newPhero;
    }
}

void ACOSolver::EASPheroUpdate(double oldPhero) {
    // iterate through legs, updating pheromones
    for(int i = 0; i < legs.size(); i++) {
        double newPhero = oldPhero;
        // newPhero = (1 - RHO) * oldPhero + deltaTotal + (deltaTauBest * ELITISM_FACTOR);
        legs[i].phero = newPhero;
    }
}

City ACOSolver::updateAntPos(Ant k) {
    double pij;
    City newCity;
    double denominator;
    for(int i = 0; i < k.unvisited.size(); i++) {
        double tempDenom = 0;
        double tempPhero = 0;
        double distToTempCity = calculateDistance(k.city.p, k.unvisited[i]);
        int j = 0;
        while(j < legs.size()) {
            if (legs[j].city1 == k.city.p && legs[j].city2 == k.unvisited[i]) {
                tempPhero = legs[j].phero;
                break;
            } else if (legs[j].city1 == k.unvisited[i] && legs[j].city2 == k.city.p) {
                tempPhero = legs[j].phero;
                break;
            }
            j++;
        }
        tempDenom = (pow(tempPhero, ALPHA) * pow((1 / distToTempCity), BETA));
        denominator += tempDenom;
    }
    while() {
        int randCityIndex = getRandomCity(k.unvisited);
        City randCity = k.unvisited[randCityIndex];
        double distToRandCity = calculateDistance(k.city.p, randCity);
        double pheroOnLegToRand;
        int iter = 0;
        while(iter < legs.size()) {
            if (legs[iter].city1 == k.city.p && legs[iter].city2 == randCity) {
                pheroOnLegToRand = legs[iter].phero;
                break;
            } else if (legs[iter].city1 == randCity && legs[iter].city2 == k.city.p) {
                pheroOnLegToRand = legs[iter].phero;
                break;
            }
            iter++;
        }
        double numerator = (pow(pheroOnLegToRand, ALPHA) * pow((1 / distToRandCity), BETA));

        pij = numerator / denominator;
        double prob = rand() / RAND_MAX;
        if (prob < pij) {
          newCity = randCity;
          k.unvisited.erase(k.unvisited.begin() + randCityIndex);
          break;
        }
    }



}

void ACOSolver::solve() {
    if (ALGTYPE == 1) {
        solveACS();
    } else {
        solveEAS();
    }
}
// NOTES
// render cities in OpenGL for lulz?
void ACOSolver::solveEAS() {
    int iterations = 0;
    while(!terminated(int iterations)) {

        iterations++;
    }
}

void ACOSolver::solveACS() {
    int iterations = 0;
    while(!terminated(int iterations)) {

        iterations++;
    }
}

bool ACOSolver::terminated(int iterations) {
    /*   terminating mode:
     *      if TERM = 1, iteration will stop after ITERATIONS iterations.
     *      if TERM = 2, iteration will stop after our solution gets within
                OPTIMAL_DEVIATION percent of the optimal solution
     *      if TERM = 3, iteration will stop after either of the above
                options are satisfied
     */
    const double TERM = 1;
    if (TERM == 1 || TERM == 3) {
        if (iterations == ITERATIONS) {
            return true;
        }
    }
    if (TERM == 2 || TERM == 3) {
        if (bsfRoute.size()/optimal < OPTIMAL_DEVIATION) {
            return true;
        }
    }

    return false;
}
