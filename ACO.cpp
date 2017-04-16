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
    double localMinTourLength = INT_MAX;
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
            // set cities to best tour cities
            bsfRoute[j] = localMinTour[j].ID;
        }
        // re-label legs in bsf
        for (int i = 0; i < cities.size(); i++) {
            for (int j = 0; j < cities.size(); j++) {
                if (inBSF(legs[i][j].city1, legs[i][j].city2)) {
                    legs[i][j].inBSF = true;
                } else {
                    legs[i][j].inBSF = false;
                }
            }
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
    return legs[cityA.ID][cityB.ID].phero;
}

// gets the pheromone level on the leg between to input cities
Leg ACOSolver::getLeg(City cityA, City cityB) {
    return legs[cityA.ID][cityB.ID];
}

bool ACOSolver::legMatchesCities(Leg theLeg, City cityA, City cityB) {
    // leg from A to B = leg from B to A, return true either way
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

bool ACOSolver::inTour(Ant a, Leg l) {
    // iterate through ant's tour
    for(int i = 0; i < a.tour.size() - 1; i++) {
        // if the leg is present, return true
        if(legMatchesCities(l, a.tour[i], a.tour[i + 1])) {
            return true;
        }
    }
    return false;
}

// initializes the 2D vector of legs
void ACOSolver::initAllLegs() {
    for (int i = 0; i < cities.size(); i++) {
        // place cities into BSF (for an intial tour)
        bsfRoute.push_back(i);
        vector<Leg> tempVect;
        for (int j = 0; j < cities.size(); j++) {
            Leg tempLeg;
            tempLeg.city1 = cities[i];
            tempLeg.city2 = cities[j];
            tempLeg.inBSF = false;
            if (i != j) {
                tempLeg.phero = PHERO_INITAL;
                tempLeg.length = calculateDistance(tempLeg.city1.p, tempLeg.city2.p);
            } else {
                tempLeg.phero = 0;
                tempLeg.length = INT_MAX;
            }

            tempVect.push_back(tempLeg);
        }
        // add to total legs
        legs.push_back(tempVect);
    }
    // guarantees this route will be replaced
    bsfRouteLength = INT_MAX;
}

// initializes the vector of ants
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
    alpha = ALPHA;
    beta = BETA;
    rho = RHO;
    readFile();
    numAnts = cities.size();

    // cout << ALGTYPE << " " << numAnts << " " << ITERATIONS << " " << PHERO_INITAL
    // << " " << OPTIMAL_DEVIATION << " " << alpha << " " << beta << " "
    // << rho << " " << ELITISM_FACTOR << " " << EPSILON << " " << Q0 << endl;
    initAllLegs();
    initAnts();
}

ACOSolver::ACOSolver(string fileName, double q0, double epsilon, double alpha, double beta, double rho) {
    srand(time(NULL));

    this->fileName = fileName;
    readFile();
    this->q_0 = q0;
    this->epsilon = epsilon;
    this->alpha = alpha;
    this->beta = beta;
    this->rho = rho;
    numAnts = cities.size();

    // cout << ALGTYPE << " " << numAnts << " " << ITERATIONS << " " << PHERO_INITAL
    // << " " << OPTIMAL_DEVIATION << " " << alpha << " " << beta << " "
    // << rho << " " << ELITISM_FACTOR << " " << EPSILON << " " << Q0 << endl;

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
            if (line.front() == '1' && started == 0) {
                started = 1;
                string entry;
                string delimiter = " ";

                City c;

                // store line number as city ID
                ID = (stoi(line.substr(0, line.find(delimiter)))) - 1;
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
                ID = (stoi(line.substr(0, line.find(delimiter)))) - 1;
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
                // cout << "The optimal is " << optimal << endl;
                break;
            }
        }
        inputFile.close();
    }

    // cout << "Printing cities! (" << cities.size() << ")" << endl;
    // for(int i = 0; i < cities.size(); i++) {
    //     printCity(cities[i]);
    // }
}

// gets the index in the unvisited vector of the next city to go to greedily
int ACOSolver::getGreedyNextCity(Ant k) {
    double currMaxValue = INT_MIN;
    int currChoiceIndex = 0;
    double distToCity = 0;
    double pheroOnLeg = 0;
    double numerator = 0;
    for (int i = 0; i < k.unvisited.size(); i++) {
        distToCity = legs[k.city.ID][k.unvisited[i].ID].length;
        pheroOnLeg = legs[k.city.ID][k.unvisited[i].ID].phero;
        numerator = pow(pheroOnLeg, alpha) * pow((1 / distToCity), beta);
        if (numerator >= currMaxValue) {
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
        for (int j = 0; j < legs[i].size(); j++) {
            if (i != j) {
                double newPhero;
                if (legs[i][j].inBSF) {
                    newPhero = ((1 - rho) * legs[i][j].phero) + rho * (1/bsfRouteLength);
                } else {
                    newPhero = (1 - rho) * legs[i][j].phero;
                }
                legs[i][j].phero = newPhero;
            }
        }
    }
}

// updates the pheromone level using the ACS local update formula
void ACOSolver::ACSLocalPheroUpdate(City cityA, City cityB) {
    // find correct leg based on cities
    int i = cityA.ID;
    int j = cityB.ID;

    // legs[i][j].phero = ((1 - EPSILON) * legs[i][j].phero) + (EPSILON * tau_0);
    legs[i][j].phero = ((1 - epsilon) * legs[i][j].phero) + (epsilon * tau_0);
    legs[j][i].phero = legs[i][j].phero;
}

// updates the pheromone level using the EAS update formula
void ACOSolver::EASPheroUpdate() {
    // iterate through legs, updating pheromones
    for(int i = 0; i < legs.size(); i++) {
        for (int j = 0; j < legs[i].size(); j++) {
            double newPhero;

            double deltaTotal = 0;
            // iterate through ants, adding 1 / tour length if this leg is in their tour
            for(int i = 0; i < ants.size(); i++) {
                if(inTour(ants[i], legs[i][j])) {
                    // add to total
                    deltaTotal += 1 / calculateTourDistance(ants[i]);
                }
            }
            double deltaTauBest = 0;
            // add if this leg is in BSF tour
            if(legs[i][j].inBSF) {
                deltaTauBest = 1 / bsfRouteLength;
            }

            newPhero = (1 - RHO) * legs[i][j].phero + deltaTotal + (deltaTauBest * ELITISM_FACTOR);

            legs[i][j].phero = newPhero;
        }
    }
}

void ACOSolver::buildTours() {
    for (int c = 0; c < cities.size() - 1; c++) {
        for (int i = 0; i < ants.size(); i++) {
            if (ALGTYPE == 1) {
                // ACS
                double prob = ((double) rand()) / RAND_MAX;
                int cityIndex;
                // if (prob < Q0) {
                if (prob < q_0) {
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
                // EAS
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
        double tempPhero = legs[k.city.ID][k.unvisited[i].ID].phero;
        double distToTempCity = legs[k.city.ID][k.unvisited[i].ID].length;
        tempDenom = (pow(tempPhero, alpha) * pow((1 / distToTempCity), beta));
        denominator += tempDenom;
    }
    int i = 0;
    while(true) {
        int randCityIndex = getRandomCity(k.unvisited);
        City randCity = k.unvisited[randCityIndex];
        double distToRandCity = legs[k.city.ID][randCity.ID].length;
        double pheroOnLegToRand = legs[k.city.ID][randCity.ID].phero;
        double numerator = (pow(pheroOnLegToRand, alpha) * pow((1 / distToRandCity), beta));

        pij = numerator / denominator;
        double prob = ((double) rand())/RAND_MAX;
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
    // cout << "tau_0 = " << tau_0 << " totalDistance = " << totalDistance << endl;
}

vector<double> ACOSolver::solve() {
    setTau();
    cout << endl;
    // if (ALGTYPE == 1) {
    //     cout << "Solving with Ant Colony System..." << endl << endl;
    // } else {
    //     cout << "Solving with Elitist Ant System..." << endl << endl;
    // }
    int iterations = 1;

    vector<double> bsf;

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
        // cout << "Best route length so far (iteration " << iterations << "): " << bsfRouteLength << endl;
        // cout << "**********************************************************************" << endl;
        // }

        bsf.push_back(bsfRouteLength);
        iterations++;
    }
    // double perc = ((double)bsfRouteLength / (double)optimal);
    // cout << endl << "Overall best: " << bsfRouteLength << " (";
    // cout << perc*100 << " percent of the optimal)." << endl;
    // cout << "Optimal (from file): " << optimal << endl;

    bsf.push_back(optimal);

    return bsf;
}

bool ACOSolver::terminated(int iterations) {
    if (TERM == 1 || TERM == 3) {
        if (iterations == ITERATIONS) {
            // cout << "Terminating because MAX number of itertaions (";
            // cout << ITERATIONS << ") met." << endl;
            return true;
        }
    }
    if (TERM == 2 || TERM == 3) {
        if ((1.0-((double)optimal/(double)bsfRouteLength)) < OPTIMAL_DEVIATION) {
            // cout << "Terminating because reached " << OPTIMAL_DEVIATION;
            // cout << " percent of optimal solution at iteration " << iterations+1 << "." << endl;
            return true;
        }
    }

    return false;
}

// legs contains duplicates (a->b = b->a)
// actually count distance for bsfDistance in initAllLegs?
// should pheromone be two way or one way?
// save some calculation time using legs as routes instead of cities (so distance is stored)
