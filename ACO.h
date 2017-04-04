#ifndef ACO_H
#define ACO_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>

using namespace std;

typedef struct _point2d {
    double x, y;
} point2D;

void printPoint(point2D p);

double calculateDistance(point2D city1, point2D city2);
bool inBSF(point2D city1, point2D city2);

class Ant {

};

class ACOSolver {
public:
    vector<point2D> cities;
    vector<point2D> bsfRoute;
    ACOSolver(string fileName);
    ~ACOSolver();

    string fileName;



private:

  // parameters to set
    // const static int ALGTYPE = 1; // 0 for EAS, 1 for ACS
    // const static int NUMANTS = 10;
    // const static int ITERATIONS = 50;
    // const double ALPHA = 1.0; // pheromone influence
    // const double BETA = 2.0; // heuristic influence
    // const double RHO = 0.1; // evaporation
    //
    // // this one only for EAS
    // const static int ELITISM_FACTOR = NUMANTS;
    //
    // // these three only for ACS
    // const double EPSILON = 0.1;
    // const double TAU0; //length of nearest neighbor tour, can we set that here? it's 1/(n·L_(nn))
    // const double Q0 = 0.9;
  // end parameters to set


    void readFile();
    double acsPheroUpdate(double oldPhero);
    double easPheroUpdate(double oldPhero);
};



#endif
