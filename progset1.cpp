#include <iostream>
#include <vector>
#include <random>

using namespace std;

vector<vector<int>> graph;
vector<vector<int>> output;


int main() {    
    // generate random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    int max_size = 10;
    int min_size = 1;
    int graph_size = rand() % max_size + min_size;

    // fill and printadjacency matrix
    for (int i = 0; i < graph_size; i++) {
        graph.push_back(*(new vector<int>));

        for (int j = 0; j < graph_size; j++) {
            graph[i].push_back(rand() % 2);
            cout << graph[i][j] << "  ";    
        }
        
        cout << endl;
    }    

    return 0;
}

void prims(int size) {
    return;
}