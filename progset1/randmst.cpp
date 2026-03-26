#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <random>
#include <cassert>
#include <array>
#include <algorithm>
#include <chrono>

using namespace std;
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::duration;
using chrono::milliseconds;

// Mersenne Twister RNG stuff. Pseudo rng for preserving independence across different trials. source: https://codereview.stackexchange.com/questions/180063/mersenne-twister-random-number-generator-function?lq=1
mt19937 rng; 
vector<vector<double>> points; // points[i]= coordinate vector for vertex i (dim >= 2)
double maxWeight = 0;
double avgWeight = 0;

// vertex definition
struct Vert {
    int rank;
    int vertex;
    Vert* parent;
};

// edge definition
struct Ed {
    Vert* start;
    Vert* end;
    double weight;
};

vector<Vert> vertices;                // set of vertices for kruskal's algorithm
vector<Ed> output;                    // set of edges for kruskal's algorithm


//-------------------------------- graph generation ----------------------------------------//

double generateUniformWeight() {
    uniform_real_distribution<double> dis(0, 1);
    return dis(rng);
}

double generateEuclideanDistance(int i, int j, int dim) {
    double inner = 0;
    for (int k = 0; k < dim; k++) {
        inner += pow(points[i][k] - points[j][k], 2.0);
    }
    return sqrt(inner);
}

void generatePoints(int num_vert, int dim) {
    uniform_real_distribution<double> dis(0, 1);

    points.clear();
    points.resize(num_vert, vector<double>(dim));

    for (int i = 0; i < num_vert; i++) {
        for (int k = 0; k < dim; k++) {
            points[i][k] = dis(rng);
        }
    }
}

bool nDimensionalCondition(int i, int j) {    
    // avoid edge to itself
    if (i == j) {
        return false;
    }
    return true;
}

bool hypercubeCondition(int i, int j) {
    // avoid edge to itself
    if (i == j) {
        return false;
    }

    int diff = abs(j - i);
    bool hyperCondition = (diff > 0) && ((diff & (diff - 1)) == 0);

    if (hyperCondition) {
        return true;
    } 
    return false;
}

//-------------------------------- kruskal's algorithm ----------------------------------------//

double findPruningThreshold(int dim, int n) {
    if (dim == 0) {
        return 2.622 * log(n) / n;
    } else if (dim == 2) {
        return 1.429 * sqrt(log(n) / n);
    } else if (dim == 3) {
        return 1.603 * pow(log(n) / n, 1.0/3.0);
    } else if (dim == 4) {
        return 1.549 * pow(log(n) / n, 0.25);
    } else {
        return 10000;  // do not prune if dim == 1
    }
}

// returns a boolean representing whether the edge weight should be pruned
bool prune(int dim, int n, double weight) {
    if (weight > findPruningThreshold(dim, n)) {
        return true;
    } else {
        return false;
    }
}

void makeSet(Vert& x) {
    vertices.push_back(x);
    vertices.back().rank = 0;
    vertices.back().parent = &vertices.back();
}

Vert* find(Vert& x) {
    if (x.vertex != (*x.parent).vertex) {
        x.parent = find(*x.parent);
    }
    return x.parent;    
}

void link(Vert& x, Vert& y) {
    if (x.rank > y.rank) {
        return link(y, x);
    } else if (x.rank == y.rank) {
        y.rank = y.rank + 1;
    }
    x.parent = &y;
    return;
}

void createUnion(Vert& x, Vert& y) {
    link(*find(x), *find(y));
    return;
}


double kruskal(int dim, int num_vert) {
    // output: X
    // vertices: set of trees
    vector<Ed> edges;  // set of edges

    vertices.reserve(num_vert); 
    // generate points for Euclidean graphs
    if (dim >= 2) {
        generatePoints(num_vert, dim);
    }

    // make set u
    for (int i = 0; i < num_vert; i++) {
        Vert add;
        add.vertex = i;
        makeSet(add);
    }

    // create set of edges and sort them
    for (int i = 0; i < num_vert; i++) {
        for (int j = i + 1; j < num_vert; j++) {
            if (dim == 1 && hypercubeCondition(i, j) || dim != 1 && nDimensionalCondition(i, j)) {
                double weight;

                // weight logic (correct models)
                if (dim == 0 || dim == 1) {
                    weight = generateUniformWeight();
                } 
                else {
                    weight = generateEuclideanDistance(i, j, dim);
                }

                if (prune(dim, num_vert, weight)) {
                    continue;
                } 

                Ed add;
                add.start = &vertices[i];
                add.end = &vertices[j];
                add.weight = weight;
                edges.push_back(add);   
            }
        }
    }

    sort(edges.begin(), edges.end(), [](const Ed& a, const Ed& b) {
        return a.weight < b.weight;
    });

    // main kruskal's algorithm logic
    int picked = 0;

    for (int i = 0; i < edges.size(); i++) {     
        if ((*find(*edges[i].start)).vertex != (*find(*edges[i].end)).vertex) {

            output.push_back(edges[i]);
            createUnion(*edges[i].start, *edges[i].end);

            picked++;

            if (picked == num_vert - 1) {
                break;
            }
        }
    }

    assert(picked == num_vert - 1);

    double total_weight = 0;
    for (int i = 0; i < output.size(); i++) {
        total_weight += output[i].weight;
        if (output[i].weight > maxWeight) {
            maxWeight = output[i].weight;
        }
    }

    avgWeight += total_weight;

    return total_weight;
}

//-------------------------------- main test logic sequence  -------------------------------------//

int main(int argc, char* argv[]) {    

    if (argc != 5) {    // not enough arguments provided
        return 0;
    }

    int testing = stoi(argv[1]);
    int numpoints = stoi(argv[2]);
    int numtrials = stoi(argv[3]);
    int dimension = stoi(argv[4]);

    // remember to clear the graph afterwards

    double result = 0;
    auto prev = high_resolution_clock::now();

    if (testing == 1) {
        for (int k = 1; k <= 4; k++) {
            cout << "---------- DIMENSION = " << k << "------------" << endl;
            for (int j = 7; j <= 18; j++) {
                numpoints = pow(2.0, j);
                for (int i = 0; i < numtrials; i++) {
                    // seed randomness once per trial
                    random_device rd;
                    rng.seed(rd());
                    
                    kruskal(k, numpoints);

                    vertices.clear();
                    output.clear();
                }     
                avgWeight = avgWeight / numtrials;

                // cout << "TRIAL for npoints = " << pow(2.0, j) << ": " << endl;
                // cout << "maximum: " << maxWeight << endl;
                duration<double, milli> time_taken = high_resolution_clock::now() - prev;

                cout << "Average Weight: " << avgWeight << " in " << time_taken.count()
                << " milliseconds" << endl;
                
                maxWeight = 0;
                avgWeight = 0;
                prev = high_resolution_clock::now();
            }
        }
        
        
    } else {
        for (int i = 0; i < numtrials; i++) {
            // seed randomness once per trial
            random_device rd;
            rng.seed(rd());
            
            result += kruskal(dimension, numpoints);

            vertices.clear();
            output.clear();
        }
        result = result/numtrials;
        // Note to Daniel: I tweaked the output format to match the format asked for in the pdf
        cout << result << " " << numpoints << " " << numtrials << " " << dimension << endl;

    }
    
    return 0;
}


/*
    // print graph (for debugging purposes)
    for (int i = 0; i < points; i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
*/