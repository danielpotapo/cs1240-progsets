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



double generateWeight(uniform_real_distribution<double> dis, mt19937 engine, int dim) {
    // weights for uniform and hypercube uniformly random 
    if (dim == 0 || dim == 1) {
        return dis(engine);
    } 

    // weights for dim >= 2 are the euclidean distance between the points
    double inner = 0;
    for (int k = 0; k < dim; k++) {
        inner += pow(dis(engine) - dis(engine), 2.0);
    }
    return sqrt(inner);
}

void nDimensionalGraph(int points) {
    
    // fill graph with points
    for (int i = 0; i < points; i++) {
        graph.push_back(*(new vector<double>));
        for (int j = 0; j < points; j++) {
            // avoid edge to itself
            if (i == j) {
                continue;
            }
            graph[i].push_back(j);
        }
    }

    // print graph (for debugging purposes)
    for (int i = 0; i < points; i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;
    cout << endl;
}

void hyperCubeGraph(int points) {
    // fill graph with points
    for (int i = 0; i < points; i++) {
        graph.push_back(*(new vector<double>));
        for (int j = 0; j < points; j++) {
            // avoid edge to itself
            if (i == j) {
                continue;
            }

            int roundedLog = (int) (log2(abs((double) (j-i))));
            bool hyperCondition = (double) pow(2, roundedLog) == (double) abs(j-i);

            if (hyperCondition) {
                graph[i].push_back(j);
            } 
        }
    }

    // print graph (for debugging purposes)
    for (int i = 0; i < points; i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}


struct Vertex {
    int rank;
    int vertex;
    Vertex* parent;
};

vector<array<double, 2>> heap;        // heap (for prims)
vector<Vertex> vertices;              // vector of linked lists




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

// E is union find set

void makeSet() {
    Vertex x;
    x.rank = 0;
    x.parent = &x;
    vertices.push_back(x);
}

Vertex* find(Vertex x) {
    if (x.vertex != (*x.parent).vertex) {
        x.parent = find(*x.parent);
    }
    return x.parent;
}

void union(Vertex x, Vertex y) {

}

Vertex link(Vertex x, Vertex y) {
    if (x.rank > y.rank) {
        return link(y, x);
    } else if (x.rank == y.rank) {
        y.rank = y.rank + 1;
    }
    x.parent = &y;
    return y;
}



int kruskal() {

}


int main(int argc, char* argv[]) {    
    int numpoints = stoi(argv[2]);
    int numtrials = stoi(argv[3]);
    int dimension = stoi(argv[4]);

    // remember to clear the graph afterwards

    cout << "hypercube" << endl;
    hyperCubeGraph(numpoints);
    // cout << "ndimensional" << endl;
    // nDimensionalGraph(numpoints);
    

    return 0;
}


/*
We will need this later.

generate random seed
random_device rd;
mt19937 engine(rd());
uniform_real_distribution<double> dis(0, 1);





*/