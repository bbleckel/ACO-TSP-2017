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

double ACOSolver::calculateTourDistance(Ant a) {
    double totalDistance = 0;
    for(int i = 0; i < a.tour.size(); i++) {
        double thisDistance = 0;
        if (i == (a.tour.size() - 1)) {
            thisDistance = calculateDistance(a.tour[i].p, a.tour[0].p)
        } else {
            thisDistance = calculateDistance(a.tour[i].p, a.tour[i+1].p)
        }
        totalDistance += thisDistance;
    }
    return totalDistance;
}

// returns the index of a random city in the city vector sent to it
int ACOSolver::getRandomCity(vector<City> cityVect) {
    int randomIndex = rand() % cityVect.size();
    return randomIndex;
}

// gets the pheromone level on the leg between to input cities
double ACOSolver::getLegPhero(City cityA, City cityB) {
  double pheroLevel = 0;
  for(int j = 0; j < legs.size(); j++) {
      if (legs[j].city1.ID == cityA.ID && legs[j].city2.ID == cityB.ID) {
          pheroLevel = legs[j].phero;
          break;
      } else if (legs[j].city1.ID == cityB.ID && legs[j].city2.ID == cityA.ID) {
          pheroLevel = legs[j].phero;
          break;
      }
  }
  return pheroLevel;
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
        bsfRoute[i] = i + 1;
        bsfRouteLength = INT_MAX;
        for (int j = i + 1; j < cities.size(); j++) {
            Leg tempLeg;
            tempLeg.city1 = cities[i];
            tempLeg.city2 = cities[j];
            tempLeg.phero = 1.0;
            legs.push_back(tempLeg);
        }
    }
}

void ACOSolver::initAnts() {
    for (int i = 0; i < NUM_ANTS; i++) {
        Ant a;
        a.unvisited = cities;
        int randCity = getRandomCity(a.unvisited);
        a.city = a.unvisited[randCity];

        ants.push_back(a);
    }
}

ACOSolver::ACOSolver(string fileName) {
    // constructor
    this->fileName = fileName;
    readFile();

    initAllLegs();
    initAnts();
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

    //now search through the file "optimals.opt.txt" to find the optimal value
    string optimalFile = "ALL_tsp/optimals.opt.txt";
    inputFile.open(optimalFile, ios::in);
    string strToRemove = "ALL_tsp/";
    string t = fileName;
    string::size_type i = t.find(strToRemove);

    if (i != string::npos) {
        t.erase(i, strToRemove.length());
    }

    string search = t.substr(0, t.find("."));
    cout << "search is: " << search << endl;

    if(!inputFile.is_open()) {
        cerr << "ERROR: Could not open file" << endl;
        exit(1);
    } else {
        while(getline(inputFile, line)) {
            if (line.substr(0, line.find(" ")) == search) {
                optimal = stoi(line.substr(line.find(": ")));
                cout << optimal << " optimal found" << endl;
                break;
            }
        }
    }

    cout << "Printing cities!" << endl;
    for(int i = 0; i < cities.size(); i++) {
        printCity(cities[i]);
    }
}

void ACOSolver::ACSPheroUpdate() {
    // iterate through legs, updating pheromones
    for(int i = 0; i < legs.size(); i++) {
        double newPhero = legs[i].phero;
        double deltaTotal = 0;

        newPhero = (1 - RHO) * legs[i].phero + deltaTotal;
        legs[i].phero = newPhero;
    }
}

void ACOSolver::EASPheroUpdate() {
    // iterate through legs, updating pheromones
    for(int i = 0; i < legs.size(); i++) {
        double newPhero = legs[i].phero;
        // newPhero = (1 - RHO) * legs[i].phero + deltaTotal + (deltaTauBest * ELITISM_FACTOR);
        legs[i].phero = newPhero;
    }
}

void ACOSolver::buildTours() {
    for (int c = 0; c < cities.size(); c++) {
        for (int i = 0; i < ants.size(); i++) {
            City city = updateAntPos(ants[i]);
            ants[i].tour.push_back(city);
        }
    }
    resetAnts();
}

void ACOSolver::resetAnts() {
    for (int i = 0; i < ants.size(); i++) {
        ants[i].unvisited = cities;
        int randCity = getRandomCity(ants[i].unvisited);
        ants[i].city = ants[i].unvisited[randCity];
    }
}

City ACOSolver::updateAntPos(Ant k) {
    double pij;
    City newCity;
    double denominator = 0;
    for(int i = 0; i < k.unvisited.size(); i++) {
        double tempDenom = 0;
        double tempPhero = getLegPhero(k.city, k.unvisited[i]);
        double distToTempCity = calculateDistance(k.city.p, k.unvisited[i].p);
        tempDenom = (pow(tempPhero, ALPHA) * pow((1 / distToTempCity), BETA));
        denominator += tempDenom;
    }
    while(true) {
        int randCityIndex = getRandomCity(k.unvisited);
        City randCity = k.unvisited[randCityIndex];
        double distToRandCity = calculateDistance(k.city.p, randCity.p);
        double pheroOnLegToRand = getLegPhero(k.city, randCity);
        double numerator = (pow(pheroOnLegToRand, ALPHA) * pow((1 / distToRandCity), BETA));

        pij = numerator / denominator;
        double prob = rand() / RAND_MAX;
        if (prob < pij) {
          newCity = randCity;
          k.unvisited.erase(k.unvisited.begin() + randCityIndex);
          break;
        }
    }

    return newCity;

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
    while(!terminated(iterations)) {
        buildTours();
        EASPheroUpdate();
        int localMinTourLength = INT_MAX;
        vector<City> localMinTour;
        //find best so far

        // go through each tour that was built and calculate total distance of tour
        for (int i = 0; i < ants.size(); i++) {
            double tourLength = calculateTourDistance(ants[i]);
            if (tourLength < localMinTourLength) {
              localMinTourLength = tourLength;
              localMinTour = ants[i].tour;
            }

        }
        // save into bsfRoute

        iterations++;
    }
}

void ACOSolver::solveACS() {
    int iterations = 0;
    while(!terminated(iterations)) {
        buildTours();
        ACSPheroUpdate();

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
    int optimal = 10628; //hard coded for att48. needs to be changed to reflect all files
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
