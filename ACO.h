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

class Ant {

};

class ACOSolver {
public:
    vector<point2D> cities;
    ACOSolver(string fileName);
    ~ACOSolver();

    string fileName;
private:
    void readFile();
};



#endif
