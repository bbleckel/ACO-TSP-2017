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

// returns true if there is a path from city1 to city2 in the bsf path
bool ACOSolver::inBSF(city city1, city city2) {
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
            leg tempLeg;
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

                city c;

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

// NOTES
// render cities in OpenGL for lulz?
