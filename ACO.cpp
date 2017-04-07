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

// calculates total distance of the tour of an ant
double ACOSolver::calculateTourDistance(Ant a) {
    double totalDistance = 0;
    for(int i = 0; i < a.tour.size(); i++) {
        double thisDistance = 0;
        if (i == (a.tour.size() - 1)) {
            thisDistance = calculateDistance(a.tour[i].p, a.tour[0].p);
        } else {
            thisDistance = calculateDistance(a.tour[i].p, a.tour[i+1].p);
        }
        totalDistance += thisDistance;
    }
    return totalDistance;
}

// updates the bsf route and length
void ACOSolver::updateBSF() {
  int localMinTourLength = INT_MAX;
  vector<City> localMinTour;

  // go through each tour that was built and calculate total distance of tour
  for (int i = 0; i < ants.size(); i++) {
      double tourLength = calculateTourDistance(ants[i]);
      if (tourLength < localMinTourLength) {
        localMinTourLength = tourLength;
        localMinTour = ants[i].tour;
      }

  }

  // replace bsfRoute if necessary
  if (localMinTourLength < bsfRouteLength) {
      bsfRouteLength = localMinTourLength;
      for (int j = 0; j < localMinTour.size(); j++) {
          bsfRoute[j] = localMinTour[j].ID;
      }
  }
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

// gets the pheromone level on the leg between to input cities
Leg ACOSolver::getLeg(City cityA, City cityB) {
  Leg theLeg;
  for(int j = 0; j < legs.size(); j++) {
      if (legs[j].city1.ID == cityA.ID && legs[j].city2.ID == cityB.ID) {
          theLeg = legs[j];
          break;
      } else if (legs[j].city1.ID == cityB.ID && legs[j].city2.ID == cityA.ID) {
          theLeg = legs[j];
          break;
      }
  }
  return theLeg;
}

bool ACOSolver::legMatchesCities(Leg theLeg, City cityA, City cityB) {
  if (theLeg.city1.ID == cityA.ID && theLeg.city2.ID == cityB.ID) {
    return true;
  } else if (theLeg.city1.ID == cityB.ID && theLeg.city2.ID == cityA.ID) {
    return true;
  }
  return false;
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
        bsfRoute.push_back(i + 1);
        bsfRouteLength = INT_MAX;
        for (int j = i + 1; j < cities.size(); j++) {
            Leg tempLeg;
            tempLeg.city1 = cities[i];
            tempLeg.city2 = cities[j];
            tempLeg.phero = PHERO_INITAL;
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
        a.unvisited.erase(a.unvisited.begin() + randCity);

        ants.push_back(a);
    }
}

ACOSolver::ACOSolver(string fileName) {
    srand(time(NULL));
    // constructor
    this->fileName = fileName;
    readFile();
    cout << ALGTYPE << " " << NUM_ANTS << " " << ITERATIONS << " " << PHERO_INITAL
            << " " << OPTIMAL_DEVIATION << " " << ALPHA << " " << BETA << " "
            << RHO << " " << ELITISM_FACTOR << " " << EPSILON << " " << Q0 << endl;
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

    if(!inputFile.is_open()) {
        cerr << "ERROR: Could not open file" << endl;
        exit(1);
    } else {
        while(getline(inputFile, line)) {
            if (line.substr(0, line.find(" ")) == search) {
                line.erase(line.begin(), line.begin()+line.find(": ")+1);
                optimal = stoi(line);
                cout << "The optimal is " << optimal << endl;
                break;
            }
        }
        inputFile.close();
    }

    cout << "Printing cities! (" << cities.size() << ")" << endl;
    for(int i = 0; i < cities.size(); i++) {
        printCity(cities[i]);
    }
}

// gets the index in the unvisited vector of the next city to go to greedily
// checked by JH 4/7, works
int ACOSolver::getGreedyNextCity(Ant k) {
    double currMaxValue = 0;
    int currChoiceIndex = 0;
    double distToCity = 0;
    double pheroOnLeg = 0;
    double numerator = 0;
    for (int i = 0; i < k.unvisited.size(); i++) {
        distToCity = calculateDistance(k.city.p, k.unvisited[i].p);
        pheroOnLeg = getLegPhero(k.city, k.unvisited[i]);
        numerator = (pow(pheroOnLeg, ALPHA) * pow((1 / distToCity), BETA));
        if (numerator > currMaxValue) {
            currMaxValue = numerator;
            currChoiceIndex = i;
        }
    }
    return currChoiceIndex;
}

// updates the pheromone level using the ACS global update formula
void ACOSolver::ACSGlobalPheroUpdate() {
    // iterate through legs, updating pheromones
    for(int i = 0; i < legs.size(); i++) {
        double newPhero = legs[i].phero;
        if (inBSF(legs[i].city1, legs[i].city2)) {
            newPhero = (1 - RHO) * legs[i].phero + RHO * (1/bsfRouteLength);
        } else {
          newPhero = (1 - RHO) * legs[i].phero;
        }
        legs[i].phero = newPhero;
    }
}

// updates the pheromone level using the ACS local update formula
void ACOSolver::ACSLocalPheroUpdate(City cityA, City cityB) {
    // find correct leg based on cities

    for(int j = 0; j < legs.size(); j++) {
        if (legMatchesCities(legs[j], cityA, cityB)) {
            legs[j].phero = ((1 - EPSILON) * legs[j].phero) + (EPSILON * tau_0);
            break;
        }
    }

}

// updates the pheromone level using the EAS update formula
void ACOSolver::EASPheroUpdate() {
    // iterate through legs, updating pheromones
    for(int i = 0; i < legs.size(); i++) {
        double newPhero = legs[i].phero;
        // newPhero = (1 - RHO) * legs[i].phero + deltaTotal + (deltaTauBest * ELITISM_FACTOR);
        legs[i].phero = newPhero;
    }
}

void ACOSolver::buildTours() {
    for (int c = 0; c < cities.size() - 1; c++) {
        for (int i = 0; i < ants.size(); i++) {
            if (ALGTYPE == 1) {
                double prob = ((double)rand())/RAND_MAX;
                int cityIndex;
                if (prob < Q0) {
                    cityIndex = getGreedyNextCity(ants[i]);
                } else {
                    cityIndex = getNextCity(ants[i]);
                }
                City city = ants[i].unvisited[cityIndex];
                City oldCity = ants[i].city;
                ants[i].city = city;
                ants[i].unvisited.erase(ants[i].unvisited.begin() + cityIndex);
                ants[i].tour.push_back(city);
                ACSLocalPheroUpdate(oldCity, city);
            } else {
                int cityIndex = getNextCity(ants[i]);
                City city = ants[i].unvisited[cityIndex];
                ants[i].city = city;
                ants[i].unvisited.erase(ants[i].unvisited.begin() + cityIndex);
                ants[i].tour.push_back(city);
            }
        }
    }
}

void ACOSolver::resetAnts() {
    for (int i = 0; i < ants.size(); i++) {
        ants[i].unvisited = cities;
        int randCity = getRandomCity(ants[i].unvisited);
        ants[i].city = ants[i].unvisited[randCity];
        ants[i].unvisited.erase(ants[i].unvisited.begin() + randCity);
        ants[i].tour.clear();
        ants[i].tour.push_back(ants[i].city);
    }
}

// gets the index of a city for the ant to go to next
int ACOSolver::getNextCity(Ant k) {
    double pij;
    double denominator = 0;
    for(int i = 0; i < k.unvisited.size(); i++) {
        double tempDenom = 0;
        double tempPhero = getLegPhero(k.city, k.unvisited[i]);
        double distToTempCity = calculateDistance(k.city.p, k.unvisited[i].p);
        tempDenom = (pow(tempPhero, ALPHA) * pow((1 / distToTempCity), BETA));
        denominator += tempDenom;
    }
    int i = 0;
    while(true) {
        int randCityIndex = getRandomCity(k.unvisited);
        City randCity = k.unvisited[randCityIndex];
        double distToRandCity = calculateDistance(k.city.p, randCity.p);
        double pheroOnLegToRand = getLegPhero(k.city, randCity);
        double numerator = (pow(pheroOnLegToRand, ALPHA) * pow((1 / distToRandCity), BETA));

        pij = numerator / denominator;
        double prob = ((double)rand())/RAND_MAX;
        if (prob < pij) {
          return randCityIndex;
        }
        i++;
    }

}

void ACOSolver::setTau() {
    vector<City> notYetVisited = cities;
    double totalDistance = 0;

    int cityIndex = getRandomCity(notYetVisited);
    int nextCityIndex = cityIndex;
    for (int c = 0; c < cities.size() - 1; c++) {
        cityIndex = nextCityIndex;
        notYetVisited.erase(notYetVisited.begin() + cityIndex);
        double minDistance = INT_MAX;

        for (int i = 0; i < notYetVisited.size(); i++) {
            double distance = calculateDistance(cities[cityIndex].p, notYetVisited[i].p);
            if (distance < minDistance) {
                minDistance = distance;
                nextCityIndex = i;
            }
        }
        totalDistance += minDistance;
    }
    tau_0 = 1/(cities.size()*totalDistance);
    cout << "tau_0 = " << tau_0 << " totalDistance = " << totalDistance << endl;
}

void ACOSolver::solve() {
    setTau();
    cout << endl;
    if (ALGTYPE == 1) {
        cout << "Solving with Ant Colony System..." << endl << endl;
    } else {
        cout << "Solving with Elitist Ant System..." << endl << endl;
    }
    int iterations = 1;
    while(!terminated(iterations)) {
        buildTours();

        updateBSF();

        if (ALGTYPE == 1) {
            ACSGlobalPheroUpdate();
        } else {
            EASPheroUpdate();
        }

        resetAnts();

        // if((ITERATIONS - iterations) % (ITERATIONS / 20) == 0 || iterations <= 10) {
            cout << "Best route length so far (iteration " << iterations << "): " << bsfRouteLength << endl;
        // }
        iterations++;
    }
    double perc = (1.0-((double)optimal/(double)bsfRouteLength));
    cout << endl << "Overall best: " << bsfRouteLength << " (";
    cout << perc*100 << " percent from optimal)." << endl;
    cout << "Optimal (from file): " << optimal << endl;
}

bool ACOSolver::terminated(int iterations) {
    if (TERM == 1 || TERM == 3) {
        if (iterations == ITERATIONS) {
            cout << "Terminating because MAX number of itertaions (";
            cout << ITERATIONS << ") met." << endl;
            return true;
        }
    }
    if (TERM == 2 || TERM == 3) {
        if ((1.0-((double)optimal/(double)bsfRouteLength)) < OPTIMAL_DEVIATION) {
            cout << "Terminating because reached " << OPTIMAL_DEVIATION;
            cout << " percent of optimal solution at iteration " << iterations+1 << "." << endl;
            return true;
        }
    }

    return false;
}
