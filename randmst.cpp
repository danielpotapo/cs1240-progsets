#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <array>

using namespace std;


/*
NOTES TO DEREK:

I've implemented part of the prim's helper function. for now, it uses a less efficient
algorithm for finding the minimum heap element, but we can optimze that later

TODO: finish prims, main logic, optimize heap minimizing


*/

vector<vector<double>> graph;         // representation of graph as an adjacency list
vector<double> distances;             // distance to each node (for prims)
vector<array<double, 2>> heap;        // heap (for prims)

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

// inefficient minimum algorithm. returns index with minimum value in heap
int findMin() {
    double min = heap[0][1];
    int minIndex = 0;
    for (int i = 1; i < heap.size(); i++) {
        if (heap[i][1] < min) {
            min = heap[i][1];
            minIndex = i;
        }
    }
    return minIndex;
}

// prim's algorithm
void prims(int point) {
    assert(point > 0);
    double prev;                  // previous node
    array<double, 2> current;     // current node
    vector<array<double, 2>> result;

    for (int i = 0; i < point; i++) {
        distances.push_back(100.0);
    }

    distances[0] = 0;
    heap.push_back({0, 0});
    prev = -1;

    while (heap.size() > 0) {
        int minIndex = findMin();
        current = heap[minIndex];
        heap.erase(heap.begin() + minIndex);
        
        for (int i = 0; i < point; i++) {
            // current[0]: node, current[1]: distance
            // skip distance to itself
            if (i == current[0]) {
                continue;
            }

            if (distances[i] > graph[current[0]][i]) {
                // do things
                // i have to go to sleep, but pr

            }

        }

    }
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
