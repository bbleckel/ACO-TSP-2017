#include "ACO.h"

using namespace std;

void printPoint(point2D p) {
    cout << "(" << p.x << ", " << p.y << ")" << endl;
}

// calculates the distance between two cities
double calculateDistance(point2D city1, point2D city2) {
    int aSquared = (city1.x - city2.x) * (city1.x - city2.x);
    int bSquared = (city1.y - city2.y) * (city1.y - city2.y);
    int cSquared = aSquared + bSquared;
    double distance = pow(cSquared, 0.5);
    return distance;
}

// returns true if there is a path from city1 to city2 in the bsf path
bool inBSF(point2D city1, point2D city2) {
    bool isInBSF = false;
    // check if the two cities are next to each other in the bsf list
    for(int i = 0; i < bsfRoute.size(); i++) {
      // have to loop to the back of the vector
      if (bsfRoute[i] == city1) {
        if (i == 0) {
          if (bsfRoute[bsfRoute.size() - 1] == city2) {
            isInBSF = true;
          }
          if (bsfRoute[1] == city2) {
            isInBSF = true;
          }

          // have to loop back to the front of the vector
        } else if (i == bsfRoute.size() - 1) {
          if (bsfRoute[bsfRoute.size() - 2] == city2) {
            isInBSF = true;
          }
          if (bsfRoute[0] == city2) {
            isInBSF = true;
          }

          // can just check either direction
        } else {
          if (bsfRoute[i - 1] == city2) {
            isInBSF = true;
          }
          if (bsfRoute[i + 1] == city2) {
            isInBSF = true;
          }
        }
      }
    }

    return isInBSF;
}

// puts all possible legs in a vector
void initAllLegs() {
    for (int i = 0; i < cities.size() - 1; i++) {
        for (int j = i + 1; j < cities.size(); j++) {
            tempLeg = new legStruct;
            tempLeg.city1 = cities[i];
            tempLeg.city2 = cities[j];
            tempLeg.phero = 0.0;
        }
    }
}

ACOSolver::ACOSolver(string fileName) {
    // constructor
    this->fileName = fileName;
    readFile();
}

ACOSolver::~ACOSolver() {
    // destructor
}

void ACOSolver::readFile() {
    // read file
    string line;
    ifstream inputFile;
    inputFile.open(fileName, ios::in);

    int x, y;
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

                // erase line number
                line.erase(0, line.find(delimiter) + delimiter.length());

                // save coordinates
                x = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());
                y = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                p.x = x;
                p.y = y;
                cities.push_back(p);

            } else if (line.front() == 'E' && line.back() == 'F') {
                // EOF found -- done
                break;
            } else if (started) {
                string entry;
                string delimiter = " ";

                // erase line number
                line.erase(0, line.find(delimiter) + delimiter.length());

                x = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());
                y = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                p.x = x;
                p.y = y;
                cities.push_back(p);

            }
        }
        inputFile.close();
    }
    cout << "Printing cities!" << endl;
    for(int i = 0; i < cities.size(); i++) {
        printPoint(cities[i]);
    }
}

double ACOSolver::acsPheroUpdate(double oldPhero){
    int newPhero = oldPhero;
    // newPhero = (1 - rho) * oldPhero + deltaTotal
    return newPhero;
}

double ACOSolver::easPheroUpdate(double oldPhero){
    int newPhero = oldPhero;
    // newPhero = (1 - rho) * oldPhero + deltaTotal + (deltaTauBest * ELITISM_FACTOR);
    return newPhero;
}

// NOTES
// if we ever need "ID" for city, change readFile to not delete line number but make it the ID
// render cities in OpenGL for lulz?
