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

    int roundedLog = (int) (log2(abs((double) (j-i))));
    bool hyperCondition = (double) pow(2, roundedLog) == (double) abs(j-i);

    if (hyperCondition) {
        return true;
    } 
    return false;

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
    link(*find(x), *find(y));
    return;
}


double kruskal(int dim, int num_vert) {
    // output: X
    // vertices: set of trees
    vector<Ed> edges;  // set of edges

    vertices.reserve(num_vert); 

    // make set u
    for (int i = 0; i < num_vert; i++) {
        Vert add;
        add.vertex = i;
        makeSet(add);
    }

    // create set of edges and sort them
    for (int i = 0; i < num_vert; i++) {
        for (int j = 0; j < num_vert; j++) {
            if (dim == 1 && hypercubeCondition(i, j) || dim != 1 && nDimensionalCondition(i, j)) {
                Ed add;
                add.start = &vertices[i];    // vertices is initially sorted in increasing order
                add.end = &vertices[j];
                add.weight = generateWeight(dim);
                edges.push_back(add);
            }
        }
    }


    sort(edges.begin(), edges.end(), [](const Ed& a, const Ed& b) {
        return a.weight < b.weight;
    });

    
    // main kruskal's algorithm logic
    for (int i = 0; i < edges.size(); i++) {     
        if ((*find(*edges[i].start)).vertex != (*find(*edges[i].end)).vertex) {
            output.push_back(edges[i]);
            createUnion(*edges[i].start, *edges[i].end);
        }
    }

    double total_weight = 0;
    for (int i = 0; i < output.size(); i++) {
        total_weight += output[i].weight;
    }

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
        
        result += kruskal(dimension, numpoints);

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