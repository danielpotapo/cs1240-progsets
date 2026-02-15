#include <iostream>
#include <vector>
#include <random>

using namespace std;

vector<vector<int>> graph;
vector<vector<int>> output;


int main() {    
    // maximum and minimum number of nodes on graph
    int max_size = 10;
    int min_size = 1;

    // generate random seed
    random_device rd;
    mt19937 engine(rd());
    uniform_int_distribution<int> int_distribution(min_size, max_size);


    int graph_size = rand() % max_size + min_size;

    // NOTE: IN PROGRESS. COMMENTED OUT BC RESULTS IN ERROR
    // fill and printadjacency matrix
    // for (int i = 0; i < graph_size; i++) {
    //     graph.push_back(*(new vector<int>));

    //     for (int j = 0; j < graph_size; j++) {
    //         // graph[i].push_back(int_distribution);
    //         cout << graph[i][j] << "  ";    
    //     }

    //     cout << endl;
    // }    

    return 0;
}

void prims(int size) {
    return;
}