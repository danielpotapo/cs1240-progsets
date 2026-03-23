/*
NOTES TO DEREK:

to run the program, run "make", then "./strassen <flag> <dimension> matrix.txt"

NOTE: I restructured some of the files; make sure you're in the progset2 directory

the first line of matrix.txt represents matrix A, second line represents matrix B

i is row, j is column

idea: add padding to bottom and right edges of matrix if it's an odd number (must prove that this works)


ptr to an element of a vector: int* ptr = &vec.at(<index>);
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAT_A 0
#define MAT_B 1

using namespace std;

vector<vector<int>> mat_a;
vector<vector<int>> mat_b;
vector<vector<int>> output;


int flag;       // user-provided flag
int dim;        // dimension of matrix 


// general idea: use the output double vector for all storage to avoid
// unnecessary allocation/deallocation


void print(vector<vector<int>> mat) {
    for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat.size(); j++) {
            cout << " " << mat[i][j] << " ";
        }
        cout << endl;
    }
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

vector<vector<int>> multiply(vector<vector<int>> one, vector<vector<int>> two) {   // start is the top left
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

    // eventually change this to indices of a single matrix
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

    string a_str;       // matrix A
    string b_str;       // matrix B

    flag = stoi(argv[1]);
    dim = stoi(argv[2]);
    string file_name = argv[3];

    fstream reader(file_name);
    if (!reader.is_open()) {
        cout << "invalid file" << endl;
        return 1;
    }

    getline(reader, a_str);
    getline(reader, b_str);
    reader.close();

    // fill vector representations of the arrays
    for (int row = 0; row < dim; row++) {
        mat_a.push_back(*(new vector<int>));
        mat_b.push_back(*(new vector<int>));
        for (int col = 0; col < dim; col++) {
            mat_a[row].push_back(a_str.at(row * dim + col) - '0');  // ascii trick to convert char to int
            mat_b[row].push_back(b_str.at(row * dim + col) - '0');
        }
    }

    // print both matrices (bug checking)
    cout << "MATRIX A" << endl;
    print(mat_a);

    cout << "MATRIX B" << endl;
    print(mat_b);

    cout << "dim: " << dim << endl;

    print(multiply(mat_a, mat_b));

    return 0;
}


/*
    for (int i = 0; i < dim; i++) {
        cout << endl;
        for (int j = 0; j < dim; j++) {
            cout << " " << b[i][j] << " ";
        }
    }



*/