#ifndef ACO_H
#define ACO_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>

using namespace std;

// parameters to set
const int ALGTYPE = 1; // 0 for EAS, 1 for ACS
const int NUM_ANTS = 10;
const int ITERATIONS = 50;
const double ALPHA = 1.0; // pheromone influence
const double BETA = 2.0; // heuristic influence
const double RHO = 0.1; // evaporation

// this one only for EAS
const int ELITISM_FACTOR = NUM_ANTS;

// these three only for ACS
const double EPSILON = 0.1;
const double TAU_0 = 0; //length of nearest neighbor tour, can we set that here? it's 1/(n·L_(nn))
const double Q0 = 0.9;
// end parameters to set

typedef struct _point2d {
    double x, y;
} point2D;

typedef struct _leg {
    point2D city1, city2;
    double phero;
} Leg;

typedef struct _city {
    point2D p;
    int ID;
} City;

void printPoint(point2D p);
void printCity(City c);

class Ant {

};

class ACOSolver {
public:
    ACOSolver(string fileName);
    ~ACOSolver();
    
    vector<City> cities;
    vector<Leg> legs;
    vector<int> bsfRoute;

    string fileName;
    
    void solveEAS();
    void solveACS();
    
    // helper functions
    bool inBSF(City city1, City city2);
    double calculateDistance(point2D city1, point2D city2);

private:
    void initAllLegs();
    void readFile();
    void ACSPheroUpdate(double oldPhero);
    void EASPheroUpdate(double oldPhero);
};



#endif
