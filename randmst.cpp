#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <array>
#include <algorithm>

using namespace std;


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

vector<vector<double>> graph;         // representation of graph as an adjacency list
vector<Vert> vertices;                // set of vertices for kruskal's algorithm
vector<Ed> output;                    // set of edges for kruskal's algorithm


//-------------------------------- graph generation ----------------------------------------//

double generateWeight(int dim) {
    random_device rd;
    mt19937 engine(rd());
    uniform_real_distribution<double> dis(0, 1);

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

    // for (int i = 0; i < graph.size(); i++) {
    //     for (int j = 0; j < graph[i].size(); j++) {
    //         cout << graph[i][j] << " ";
    //     }
    //     cout << endl;
    // }
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

}

//-------------------------------- kruskal's algorithm ----------------------------------------//

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
    cout << "union between " << x.vertex << " and " << y.vertex << endl;
    link(*find(x), *find(y));
    return;
}


double kruskal(int dim) {
    // output: X
    // vertices: set of trees
    vector<Ed> edges;  // set of edges

    vertices.reserve(graph.size()); 

    // make set u
    for (int i = 0; i < graph.size(); i++) {
        Vert add;
        add.vertex = i;
        makeSet(add);
    }

    // create set of edges and sort them
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            Ed add;
            add.start = &vertices[i];    // vertices is initially sorted in increasing order
            add.end = &vertices[j];
            add.weight = generateWeight(dim);
            edges.push_back(add);
        }
    }


    sort(edges.begin(), edges.end(), [](const Ed& a, const Ed& b) {
        return a.weight < b.weight;
    });

    
    // main kruskal's algorithm logic
    for (int i = 0; i < edges.size(); i++) {     
        cout << i << endl;   
        if ((*find(*edges[i].start)).vertex != (*find(*edges[i].end)).vertex) {
            cout << "edges " << (*find(*edges[i].start)).vertex << " vs " << (*find(*edges[i].end)).vertex << endl;
            output.push_back(edges[i]);
            createUnion(*edges[i].start, *edges[i].end);
        }
    }

    double total_weight = 0;
    for (int i = 0; i < output.size(); i++) {
        total_weight += output[i].weight;
    }

    cout << "MINIMUM SPANNING WEIGHTS" << endl;

    for (int i = 0; i < output.size(); i++) {
        cout << output[i].weight << endl;
    }
    cout << "NEXT TRIAL" << endl;

    return total_weight;
}

//-------------------------------- main test logic sequence  -------------------------------------//

int main(int argc, char* argv[]) {    
    int numpoints = stoi(argv[2]);
    int numtrials = stoi(argv[3]);
    int dimension = stoi(argv[4]);

    // remember to clear the graph afterwards

    double result = 0;
    for (int i = 0; i < numtrials; i++) {
        if (dimension == 1) {
            hyperCubeGraph(numpoints);
        } else {
            nDimensionalGraph(numpoints);
        }
        
        result += kruskal(dimension);

        graph.clear();
        vertices.clear();
        output.clear();
    }

    result = result/numtrials;

    cout << "Final Result: " << result << endl;
    
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