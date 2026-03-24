/*
NOTES TO DEREK:


to run the program, run "make", then "./strassen <flag> <dimension> matrix.txt"


NOTE: I restructured some of the files; make sure you're in the progset2 directory


the first line of matrix.txt represents matrix A, second line represents matrix B


i is row, j is column


idea: add padding to bottom and right edges of matrix if it's an odd number (must prove that this works)




ptr to an element of a vector: int* ptr = &vec.at(<index>);
*/



#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <chrono>
#include <algorithm>
#include <cmath>    


using namespace std;
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::duration;
using chrono::milliseconds;

vector<vector<int>> mat_a;
vector<vector<int>> mat_b;
vector<vector<int>> output;
mt19937 rng; 



int flag;       // user-provided flag
int dim;        // dimension of matrix
int threshold;



void print(vector<vector<int>> mat) {
    for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat.size(); j++) {
            cout << " " << mat[i][j] << " ";
        }
        cout << endl;
    }
}


void print_diag(vector<vector<int>> mat) {
    for (int i = 0; i < mat.size(); i++) {
        cout << mat[i][i] << endl;
    }
}

int add_diag(vector<vector<int>> mat) {
    int sum = 0;
    for (int i = 0; i < mat.size(); i++) {
        sum += mat[i][i];
    }
    return sum;
}


vector<vector<int>> generate(double p) {
    vector<vector<int>> output;
    uniform_real_distribution<double> dis(0, 1);

    for (int i = 0; i < 1024; i++) {
        output.push_back(vector<int>());
        for (int j = 0; j < 1024; j++) {
            if (dis(rng) <= p) {
                output[i].push_back(1);
            } else {
                output[i].push_back(0);
            }
        }
    }

    return output;
}


vector<vector<int>> add(vector<vector<int>> one, vector<vector<int>> two, int size) {
    vector<vector<int>> result = one;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = one[i][j] + two[i][j];
        }
    }
    return result;
}


vector<vector<int>> subtract(vector<vector<int>> one, vector<vector<int>> two, int size) {
    vector<vector<int>> result = one;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = one[i][j] - two[i][j];
        }
    }
    return result;
}


// start_col, start_row is the top left element

vector<vector<int>> naive_multiply(vector<vector<int>> one, vector<vector<int>> two) {
    vector<vector<int>> result = one;

    for (int row = 0; row < one.size(); row++) {
        for (int col = 0; col < one.size(); col++) {
            int sum = 0;
            for (int element = 0; element < one.size(); element++) {
                sum += (one[row][element] * two[element][col]);
            }
            result[row][col] = sum;   
        }
    }

    return result;
}


vector<vector<int>> multiply(vector<vector<int>> one, vector<vector<int>> two) {   // start is the top left
    
    if (one.size() <= threshold) {
        return naive_multiply(one, two);
    }
    
    int prune = 0;
    vector<vector<int>> result = one;

    if (one.size() % 2 != 0 && one.size() != 1) {
        prune = 1;
        one.push_back(vector<int>());
        two.push_back(vector<int>());


        for (int i = 0; i < one[0].size(); i++) {
            one[one.size() - 1].push_back(0);
            two[two.size() - 1].push_back(0);
        }


        for (int i = 0; i < one.size(); i++) {
            one[i].push_back(0);
            two[i].push_back(0);
        }
    }

    int size = one.size();


     // base case
    if (size == 1) {
        result[0][0] *= two[0][0];
        return result;
    }


    vector<vector<int>> a;
    vector<vector<int>> b;
    vector<vector<int>> c;
    vector<vector<int>> d;
    vector<vector<int>> e;
    vector<vector<int>> f;
    vector<vector<int>> g;
    vector<vector<int>> h;

    for (int i = 0; i < size/2; i++) {
        a.push_back(vector<int>());
        e.push_back(vector<int>());
        for (int j = 0; j < size/2; j++) {
            a[i].push_back(one[i][j]);
            e[i].push_back(two[i][j]);
        }
    }


    for (int i = size/2; i < size; i++) {
        c.push_back(vector<int>());
        g.push_back(vector<int>());
        for (int j = 0; j < size/2; j++) {
            c[i - size/2].push_back(one[i][j]);
            g[i - size/2].push_back(two[i][j]);
        }
    }


    for (int i = 0; i < size/2; i++) {
        b.push_back(vector<int>());
        f.push_back(vector<int>());
        for (int j = size/2; j < size; j++) {
            b[i].push_back(one[i][j]);
            f[i].push_back(two[i][j]);
        }
    }


    for (int i = size/2; i < size; i++) {
        d.push_back(vector<int>());
        h.push_back(vector<int>());
        for (int j = size/2; j < size; j++) {
            d[i - size/2].push_back(one[i][j]);
            h[i - size/2].push_back(two[i][j]);
        }
    }


    // this will have to change later for cases that aren't multiples of 2
    vector<vector<int>> p1 = multiply(a, subtract(f, h, size/2));
    vector<vector<int>> p2 = multiply(add(a, b, size/2), h);
    vector<vector<int>> p3 = multiply(add(c, d, size/2), e);
    vector<vector<int>> p4 = multiply(d, subtract(g, e, size/2));
    vector<vector<int>> p5 = multiply(add(a, d, size/2), add(e, h, size/2));
    vector<vector<int>> p6 = multiply(subtract(b, d, size/2), add(g, h, size/2));
    vector<vector<int>> p7 = multiply(subtract(c, a, size/2), add(e, f, size/2));


    vector<vector<int>> top_left = add(add(p4, p5, size/2), subtract(p6, p2, size/2), size/2);
    vector<vector<int>> top_right = add(p1, p2, size/2);
    vector<vector<int>> bottom_left = add(p3, p4, size/2);
    vector<vector<int>> bottom_right = add(add(p5, p7, size/2), subtract(p1, p3, size/2), size/2);


    for (int i = 0; i < size - prune; i++) {
        for (int j = 0; j < size - prune; j++) {
            if (i < size/2 && j < size/2) {
                result[i][j] = top_left[i][j];
            } else if (i < size/2 && j >= size/2) {
                result[i][j] = top_right[i][j - size/2];
            } else if (i >= size/2 && j < size/2) {
                result[i][j] = bottom_left[i - size/2][j];
            } else {
                result[i][j] = bottom_right[i - size/2][j - size/2];
            }
        }
    }    
    return result;
}


int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "not enough args" << endl;
        return 1;
    }

    flag = stoi(argv[1]);
    dim = stoi(argv[2]);

    // part 3
    if (flag == 2) {
        for (int i = 1; i <= 5; i++) {
            vector<vector<int>> vec = generate(0.01 * i);
            vector<vector<int>> result = multiply(vec, multiply(vec, vec));

            cout << "for number " << i << ": " << add_diag(result)/6 << endl;
        }

        return 0;
    }


    // run tests
    if (flag == 1) {
        for (int j = 0; j < 2; j++) {
            int shortest_val = 1000;
            double shortest_time = 1000;
            mat_a.clear();
            mat_b.clear();
            uniform_real_distribution<double> dis(-9, 9);

            for (int i = 0; i < dim; i++) {
                mat_a.push_back(vector<int>());
                mat_b.push_back(vector<int>());
                for (int j = 0; j < dim; j++) {
                    mat_a[i].push_back(dis(rng));
                    mat_b[i].push_back(dis(rng));  
                }
            }

            auto previous = high_resolution_clock::now();
            naive_multiply(mat_a, mat_b);
            duration<double, milli> naive_time = high_resolution_clock::now() - previous;
            cout << "naive time taken: " << naive_time.count() << endl;

            for (int i = 1; i < dim/2; i++) {
                threshold = i;
                previous = high_resolution_clock::now();
                multiply(mat_a, mat_b);
                duration<double, milli> time_taken = high_resolution_clock::now() - previous;
                if (time_taken.count() < shortest_time) {
                    shortest_time = time_taken.count();
                    shortest_val = i;
                }
                cout << "time taken for " << i << ": " << time_taken.count() << endl;
            }
            cout << "SHORTEST VALUE: " << shortest_val << endl;
        }
        return 0;
    } 


    // run multiplication
    string line;
    string file_name = argv[3];
    fstream reader(file_name);

    if (!reader.is_open()) {
        cout << "invalid file" << endl;
        return 1;
    }    

    // fill vector representations of the arrays
    for (int row = 0; row < dim; row++) {
        mat_a.push_back(vector<int>());
        for (int col = 0; col < dim; col++) {
            getline(reader, line);
            mat_a[row].push_back(stoi(line));  // ascii trick to convert char to int
        }
    }


    for (int row = 0; row < dim; row++) {
        mat_b.push_back(vector<int>());
        for (int col = 0; col < dim; col++) {
            getline(reader, line);
            mat_b[row].push_back(stoi(line));
        }
    }

    reader.close();

    // print both matrices (bug checking)
    // cout << "MATRIX A" << endl;
    // print(mat_a);


    // cout << "MATRIX B" << endl;
    // print(mat_b);


    // cout << "OUTPUT" << endl;

    threshold = 15;
    print_diag(multiply(mat_a, mat_b));



    return 0;
}
