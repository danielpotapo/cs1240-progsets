#include <iostream>
#include <vector>
#include <random>
#include <cassert>

using namespace std;

// formatting as an adjacency list
vector<vector<double>> graph;

void fillGraph(int points) {
    // generate random seed
    random_device rd;
    mt19937 engine(rd());
    uniform_real_distribution<double> dis(0, 1);

    // fill graph with points
    for (int i = 0; i < points; i++) {
        graph.push_back(*(new vector<double>));
        for (int j = 0; j < points; j++) {
            graph[i].push_back(dis(engine));
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

int main(int argc, char* argv[]) {    

    int numpoints = stoi(argv[2]);
    int numtrials = stoi(argv[3]);
    int dimension = stoi(argv[4]);

    // dummy variables for testing purposes
    for (int i = 0; i < numtrials; i++) {
        fillGraph(numpoints);
    }

    return 0;
}
