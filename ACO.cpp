#include "ACO.h"

using namespace std;

void printPoint(point2D p) {
    cout << "(" << p.x << ", " << p.y << ")" << endl;
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
                
                // save coordinates
                x = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());
                y = stof(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());
                
                p.x = x;
                p.y = y;
                cities.push_back(p);
                
            } else if (line.front() == 'E' && line.back() == 'F') {
                // done
                break;
            } else if (started) {
                string entry;
                string delimiter = " ";
                
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



// render cities in OpenGL for lulz?
