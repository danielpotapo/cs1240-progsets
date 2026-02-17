#include <iostream>
#include <vector>
#include <random>
#include <cassert>

using namespace std;


/*
NOTES TO DEREK:

I've implemented setup for all of the n dimensional graphs in the helper function nDimensionalGraph

I'm planning for the main logic to occur in main, calling helper functions as necessary

TODO: add a helper function for Prim's algorithm, implement function hyperCubeGraph


*/

// formatting as an adjacency list
vector<vector<double>> graph;

void nDimensionalGraph(int points, int dim) {
    // generate random seed
    random_device rd;
    mt19937 engine(rd());
    uniform_real_distribution<double> dis(0, 1);

    // fill graph with points
    for (int i = 0; i < points; i++) {
        graph.push_back(*(new vector<double>));
        for (int j = 0; j < points; j++) {
            double inner = 0; 

            // set weight as distance for n dimensions
            for (int k = 0; k < dim; k++) {
                inner += pow(dis(engine) - dis(engine), 2.0);
            }
            graph[i].push_back(sqrt(inner));
        }
    }

    // print graph (for debugging purposes)
    for (int i = 0; i < points; i++) {
        for (int j = 0; j < points; j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}

void hyperCubeGraph(int points) {

}





int main(int argc, char* argv[]) {    
    int numpoints = stoi(argv[2]);
    int numtrials = stoi(argv[3]);
    int dimension = stoi(argv[4]);

    // dummy variables for testing purposes
    for (int i = 0; i < numtrials; i++) {
        nDimensionalGraph(numpoints, 2);
    }

    return 0;
}
