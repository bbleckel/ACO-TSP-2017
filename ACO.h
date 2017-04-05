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
const int NUM_ANTS = 10; // number of ants
const int ITERATIONS = 50; // number of iterations
const double OPTIMAL_DEVIATION = 0.01; // percentage from the optimal
                                        //within which our solution will stop iterating
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

typedef struct _city {
    point2D p;
    int ID;
} City;

typedef struct _leg {
    City city1, city2;
    double phero;
} Leg;

void printPoint(point2D p);
void printCity(City c);

typedef struct _ant {
    City city;
    vector<City> unvisited;
    vector<City> tour;
} Ant;

class ACOSolver {
public:
    ACOSolver(string fileName);
    ~ACOSolver();

    vector<Ant> ants;
    vector<City> cities;
    vector<Leg> legs;
    vector<int> bsfRoute;
    int bsfRouteLength;

    string fileName;
    int optimal;

    /* solver functions */
    void solve();
    void solveEAS();
    void solveACS();
    bool terminated(int iterations);

    // helper functions
    bool inBSF(City city1, City city2);
    double calculateDistance(point2D City1, point2D City2);
    int getRandomCity(vector<City> unvisited);
    double getLegPhero(City city1, City city2);
    double calculateTourDistance(Ant a);

private:
    void initAllLegs();
    void initAnts();
    void resetAnts();
    void readFile();
    void ACSPheroUpdate();
    void EASPheroUpdate();
    void buildTours();
    City updateAntPos(Ant a);
};



#endif
