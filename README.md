# ACO-TSP-2017
By: Bo Bleckel, Jasper Houston, and Dylan Parsons
Nature Inspired Computation - CSCI 3445
Bowdoin College

This program is implemented in C++.

Writeup can be found at [Ants.pdf](Ants.pdf)

This program solves a given Traveling Salesman Problem (TSP) using an Ant Colony Optimization (ACO) approach.
The goal of the program is to compare Elitist Ant System (EAS) and Ant Colony System (ACS).
A detailed explanation of TSP, ACO, our methods, and results are in the included paper entitled ACO.pdf.
The main.cpp file deals with the testing and with parsing the command line.
The command line arguments are as follows, in the order presented:

### Instructions
For all Ant Colony Optimization:
./main filename
filename = path to the tsp file to use
Sample input: './main ALL_tsp/berlin52.tsp'

The path to the file is entered after the executable for the file, which is ./main
and can be compiled using the included Makefile (simply type make to compile).
Similarly, type “make clean” to remove all executables created by the make file.

The algorithms are implemented using a class for the ACO Solver and a number of
structs representing ants, cities, and legs between cities.

All parameters are set in the ACO.h file as global constants.

There is a test() function that can be used to loop through different values for
the parameters. This is not useful for most users, as even running a single trial
can take hours depending on the TSP file size, but for more extensive testing it
would be extremely helpful. It was critical in determining the optimal parameters
to be used in comparing ACS and EAS.
