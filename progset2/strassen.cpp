/*
NOTES TO DEREK:


to run the program, run "make", then "./strassen <flag> <dimension> matrix.txt"

PROGRAM COMMANDS:
"./strassen 0 <dimension> matrix.txt": run strassen's on a matrix from matrix.txt of size dimension

"./strassen 1 <dimension> matrix.txt": average of 100 trials of strasses on a matrix of size dimension.
note that the 0 index corresponds to the naive algorithm, and indices 1-10 are skipped b/c they take
too long

"./strassen 2 <dimension> matrix.txt": run a single trial of part 3
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

vector<long> mat_a;
vector<long> mat_b;
vector<long> mat_c;
vector<long> mat_d;
mt19937 rng; 



int flag;       // user-provided flag
int dim;        // dimension of matrix
int threshold;



int index(int i, int j, int stride) {
    return i * stride + j;
}

long element(int i, int j, long* mat, int stride) {
    return *(mat + index(i, j, stride));
}

int get_pow(int dimension) {
    double exponent = 0;
    while (pow(2, exponent) < dimension) {
        exponent++;
    }
    return (int) pow(2, exponent);
}




void print(long* mat, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << " " << element(i, j, mat, size) << " ";
        }
        cout << endl;
    }
}


void print_diag(long* mat, int size) {
    for (int i = 0; i < size; i++) {
        cout << element(i, i, mat, size) << endl;
    }
}

long add_diag(long* mat, int size) {
    long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += element(i, i, mat, size);
    }
    return sum;
}


long* generate(double p) {
    long* output = new long[1024 * 1024];
    uniform_real_distribution<double> dis(0, 1);

    for (int i = 0; i < 1024 * 1024; i++) {
        output[i] = 0;
    }

    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < i; j++) {
            if (dis(rng) <= p) {
                output[index(i, j, 1024)] = 1;
                output[index(j, i, 1024)] = 1;
            }
        }
    }

    return output;
}


long* add(long* one, long* two, int size, int s1, int s2) {
    long* result = new long[size * size];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[index(i, j, size)] = one[index(i, j, s1)] + two[index(i, j, s2)];
        }
    }

    return result;
}

long* subtract(long* one, long* two, int size, int s1, int s2) {
    long* result = new long[size * size];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[index(i, j, size)] = one[index(i, j, s1)] - two[index(i, j, s2)];
        }
    }

    return result;
}


long* naive_multiply(long* one, long* two, int size, int s1, int s2) {
    long* result = new long[size * size];

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            long sum = 0;
            for (int e = 0; e < size; e++) {
                sum += element(row, e, one, s1) * element(e, col, two, s2);
            }
            result[index(row, col, size)] = sum;   
        }
    }

    return result;
}


long* multiply(long* one, long* two, int size, int s1, int s2) {   // start is the top left
    
    if (size <= threshold) {
        return naive_multiply(one, two, size, s1, s2);
    }
    
    int prune = 0;

    long* result = new long[size * size];


    long* a = &one[index(0, 0, s1)];
    long* b = &one[index(0, size/2, s1)];
    long* c = &one[index(size/2, 0, s1)];
    long* d = &one[index(size/2, size/2, s1)];
    long* e = &two[index(0, 0, s2)];
    long* f = &two[index(0, size/2, s2)];
    long* g = &two[index(size/2, 0, s2)];
    long* h = &two[index(size/2, size/2, s2)];


    long* p1_1 = subtract(f, h, size/2, s2, s2);
    long* p1 = multiply(a, p1_1, size/2, s1, size/2);

    long* p2_1 = add(a, b, size/2, s1, s1);
    long* p2 = multiply(p2_1, h, size/2, size/2, s2);

    long* p3_1 = add(c, d, size/2, s1, s1);
    long* p3 = multiply(p3_1, e, size/2, size/2, s2);

    long* p4_1 = subtract(g, e, size/2, s2, s2);
    long* p4 = multiply(d, p4_1, size/2, s1, size/2);

    long* p5_1 = add(a, d, size/2, s1, s1);
    long* p5_2 = add(e, h, size/2, s2, s2);
    long* p5 = multiply(p5_1, p5_2, size/2, size/2, size/2);

    long* p6_1 = subtract(b, d, size/2, s1, s1);
    long* p6_2 = add(g, h, size/2, s2, s2);
    long* p6 = multiply(p6_1, p6_2, size/2, size/2, size/2);

    long* p7_1 = subtract(c, a, size/2, s1, s1);
    long* p7_2 = add(e, f, size/2, s2, s2);
    long* p7 = multiply(p7_1, p7_2, size/2, size/2, size/2);


    long* tl_1 = add(p4, p5, size/2, size/2, size/2);
    long* tl_2 = subtract(p6, p2, size/2, size/2, size/2);
    long* top_left = add(tl_1,tl_2 , size/2, size/2, size/2);

    long* top_right = add(p1, p2, size/2, size/2, size/2);
    long* bottom_left = add(p3, p4, size/2, size/2, size/2);

    long* br_1 = add(p5, p7, size/2, size/2, size/2);
    long* br_2 = subtract(p1, p3, size/2, size/2, size/2);
    long* bottom_right = add(br_1, br_2, size/2, size/2, size/2);


    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i < size/2 && j < size/2) {
                result[index(i, j, size)] = element(i, j, top_left, size/2);
            } else if (i < size/2 && j >= size/2) {
                result[index(i, j, size)] = element(i, j - size/2, top_right, size/2);
            } else if (i >= size/2 && j < size/2) {
                result[index(i, j, size)] = element(i - size/2, j, bottom_left, size/2);
            } else {
                result[index(i, j, size)] = element(i - size/2, j - size/2, bottom_right, size/2);
            }
        }
    }    

    delete[] p1;
    delete[] p1_1;
    delete[] p2;
    delete[] p2_1;
    delete[] p3;
    delete[] p3_1;
    delete[] p4;
    delete[] p4_1;
    delete[] p5;
    delete[] p5_1;
    delete[] p5_2;
    delete[] p6;
    delete[] p6_1;
    delete[] p6_2;
    delete[] p7;
    delete[] p7_1;
    delete[] p7_2;
    delete[] top_left;
    delete[] tl_1;
    delete[] tl_2;
    delete[] top_right;
    delete[] bottom_left;
    delete[] bottom_right;
    delete[] br_1;
    delete[] br_2;

    return result;
}

long* mat_mult(long* one, long* two, int size, int s1, int s2, int cut) {
    long* multiplication = multiply(one, two, size, s1, s2);

    if (cut == 0) {
        return multiplication;
    }

    int new_size = size - cut;
    long* result = new long[new_size * new_size];

    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            result[index(i, j, new_size)] = element(i, j, multiplication, size);
        }
    }
    delete[] multiplication;

    return result;
}


int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "not enough args" << endl;
        return 1;
    }

    flag = stoi(argv[1]);
    dim = stoi(argv[2]);
    int total_dim = get_pow(dim);
    int to_cut = total_dim - dim;

    

    // part 3
    if (flag == 2) {
        threshold = 56;
        for (int i = 1; i <= 5; i++) {
            long* vec = generate(0.01 * i);
            dim = total_dim;
            assert(dim == 1024);
            
            long* first_result = mat_mult(vec, vec, dim, dim, dim, 0);
            long* result = mat_mult(vec, first_result, dim, dim, dim, 0);

            cout << "for value " << ((double) i) * 0.01 << ": " << add_diag(result, dim)/6 << endl;
            
            delete[] vec;
            delete[] first_result;
            delete[] result;
        }

        return 0;
    }


    // run tests
    if (flag == 1) {
        vector<double> counts;

        for (int i = 0; i < dim/2; i++) {
            counts.push_back(0);
        }

        for (int j = 1; j <= 100; j++) {
            int shortest_val = 1000;
            double shortest_time = 1000;
            mat_a.clear();
            mat_b.clear();
            mat_c.clear();
            mat_d.clear();
            uniform_real_distribution<double> dis(-9, 9);

            // fill matrices
            for (int i = 0; i < total_dim ; i++) {
                for (int j = 0; j < total_dim; j++) {
                    if (i < dim && j < dim) {
                        int num1 = dis(rng);
                        int num2 = dis(rng);

                        mat_a.push_back(num1);
                        mat_b.push_back(num2);  
                        mat_c.push_back(num1);
                        mat_d.push_back(num2);
                    } else {
                        mat_c.push_back(0);
                        mat_d.push_back(0);
                    }
                }
            }

            // naive test
            auto previous = high_resolution_clock::now();

            long* naive = naive_multiply(mat_a.data(), mat_b.data(), dim, dim, dim);
            duration<double, milli> naive_time = high_resolution_clock::now() - previous;
            delete[] naive;
            counts[0] += naive_time.count();

            // general test
            for (int i = 10; i < dim/2; i++) {
                threshold = i;

                // calculate time
                previous = high_resolution_clock::now();
                long* general = mat_mult(mat_c.data(), mat_d.data(), dim, dim, dim, to_cut);
                duration<double, milli> time_taken = high_resolution_clock::now() - previous;
                delete[] general;

                counts[i] += time_taken.count();
            }
            cout << "just finished trial " << j << endl;
        }

        for (int i = 0; i < counts.size(); i++) {
            cout << "count of " << i << ": " << counts[i] / 100 << endl;
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
    for (int i = 0; i < total_dim; i++) {
        for (int j = 0; j < total_dim; j++) {
            if (j >= dim || i >= dim) {
                mat_a.push_back(0);
            } else {
                getline(reader, line);
                mat_a.push_back(stoi(line));  
            }
        }
    }

    for (int i = 0; i < total_dim; i++) {
        for (int j = 0; j < total_dim; j++) {
            if (j >= dim || i >= dim) {
                mat_b.push_back(0);
            } else {
                getline(reader, line);
                mat_b.push_back(stoi(line));  
            }
        }
    }

    dim = total_dim;
    reader.close();

    

    // print both matrices (bug checking)
    // cout << "MATRIX A" << endl;
    // print(mat_a.data(), dim);


    // cout << "MATRIX B" << endl;
    // print(mat_b.data(), dim);


    // cout << "OUTPUT" << endl;

    threshold = 5;
    print_diag(mat_mult(mat_a.data(), mat_b.data(), dim, dim, dim, to_cut), dim - to_cut);

    return 0;
}