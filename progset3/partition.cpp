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

static mt19937 rng(random_device{}());
vector<long long> values;

void print_vector(vector<double> v) {
    for (long long i = 0; i < v.size(); i++) {
        cout << v[i] << "   ";
    }
    cout << endl;
}

// return index of left element of heap (-1 if not in heap)
long long left(vector<long long>& heap, long long index) {
    if (index * 2 + 1 >= heap.size()) {
        return -1;
    }
    return 2 * index + 1;
}

// return index of right element of heap (-1 if not in heap)
long long right(vector<long long>& heap, long long index) {
    if (index * 2 + 2 >= heap.size()) {
        return -1;
    }
    return 2 * index + 2;
}

void swap(vector<long long>& heap, long long i, long long j) {
    long long temp = heap[i];

    heap[i] = heap[j];
    heap[j] = temp;
}

// n is the index of the element we're min heapifying
void max_heapify(vector<long long>& heap, long long n) {
    long long largest;
    long long l = left(heap, n);
    long long r = right(heap, n);
    if (l != -1 && heap[l] > heap[n]) {
        largest = l;
    } else {
        largest = n;
    }

    if (r != -1 && heap[r] > heap[largest]) {
        largest = r;
    }

    if (largest != n) {
        swap(heap, n, largest);
        max_heapify(heap, largest);
    }
}

void build_heap(vector<long long>& heap) {
    for (long long i = heap.size()/2 - 1; i >= 0; i--) {
        max_heapify(heap, i);
    }
}

long long extract_max(vector<long long>& heap) {
    long long max = heap[0];
    heap[0] = heap[heap.size() - 1];
    heap.pop_back();
    max_heapify(heap, 0);
    return max;
}

long long peek(vector<long long>& heap) {
    return heap[0];
}

long long kk(vector<long long> heap) {
    if (heap.size() == 2) {
        return abs(heap[1] - heap[0]);
    } else if (heap.size() == 1) {
        return abs(heap[0]);
    } else if (heap.size() < 1) {
        return -1;
    }

    while (heap[1] != 0 || heap[2] != 0) {
        long long largest = heap[0];
        long long second_largest = -1;
        long long sl_index = left(heap, 0);

        if (left(heap, 0) != -1) {
            second_largest = heap[left(heap, 0)];
        }

        // do triangle comparison to find largest and second-largest elements in heap
        if (right(heap, 0) != -1 && heap[right(heap, 0)] > second_largest) {
            second_largest = heap[right(heap, 0)];
            sl_index = right(heap, 0);
        }

        if (second_largest == -1) {
            assert(false);             // something has gone very wrong!
        }

        heap[0] = largest - second_largest;
        heap[sl_index] = 0;

        max_heapify(heap, sl_index);
        max_heapify(heap, 0);
    }
    
    max_heapify(heap, 0);

    return abs(heap[0]);
}

long long compute_residue(vector<long long> v, vector<long long> signs) {
    long long total_residue = 0;

    assert(v.size() == signs.size());

    for (long long i = 0; i < v.size(); i++) {
        total_residue += v[i] * signs[i];
    }

    return abs(total_residue);
}

vector<long long> generate_random_sign(vector<long long> v) {
    uniform_int_distribution<> distribution(0, 1);
    vector<long long> signs; 

    for (long long i = 0; i < v.size(); i++) {
        long long rand = distribution(rng) * 2 - 1;
        signs.push_back(rand);
    }

    return signs;
}

long long random_sign_residue(vector<long long> v) {
    vector<long long> signs = generate_random_sign(v);

    return compute_residue(v, signs);
}

vector<long long> neighbor(vector<long long> v) {
    uniform_int_distribution<> distribution(0, v.size() - 1);
    uniform_int_distribution<> smaller_distribution(0, v.size() - 2);
    uniform_int_distribution<> half(0, 1);

    vector<long long> result = v;
    
    long long index_1 = distribution(rng);
    long long index_2 = smaller_distribution(rng);

    if (index_2 >= index_1) {
        index_2 += 1;
    }

    result[index_1] = -1 * result[index_1];

    if (half(rng) == 1) {
        result[index_2] = -1 * result[index_2];
    }

    return result;
}

vector<long long> pre_neighbor(vector<long long> v, vector<long long> current_best_p) {
    uniform_int_distribution<> distribution(0, v.size() - 1);
    uniform_int_distribution<> smaller_distribution(0, v.size() - 2);

    vector<long long> result = current_best_p;

    long long element_index = distribution(rng);
    long long new_bucket_index = smaller_distribution(rng);

    if (new_bucket_index >= current_best_p[element_index]) {
        new_bucket_index += 1;
    }

    result[element_index] = new_bucket_index;

    return result;
}

long long repeated_random(vector<long long> v) {    
    long long minimum = random_sign_residue(v);
    for (long long i = 0; i < 25000; i++) {
        long long current = abs(random_sign_residue(v));
        if (abs(current) < minimum) {
            minimum = abs(current);
        }
    }
    return minimum;
}


long long hill_climbing(vector<long long> v) {
    vector<long long> current_best = generate_random_sign(v);   // this is a sign array, v is your vals

    for (long long i = 0; i < 25000; i++) {
        vector<long long> temp = neighbor(current_best);
        if (compute_residue(v, temp) < compute_residue(v, current_best)) {
            current_best = temp;
        }
    }

    return compute_residue(v, current_best);
}

double temperature(long long iter) {
    return pow(10, 10)* pow(0.8, floor(iter / 300));
}

long long simulated_annealing(vector<long long> v) {
    vector<long long> s = generate_random_sign(v);
    vector<long long> current_best = s;
    uniform_real_distribution<> distribution(0, 1);

    for (long long iter = 0; iter < 25000; iter++) {
        vector<long long> s_prime = neighbor(s);

        if (compute_residue(v, s_prime) < compute_residue(v, s)) {
            s = s_prime;
        } else if (distribution(rng) <= exp(-1 * ((compute_residue(v, s_prime) - compute_residue(v, s)) / temperature(iter)))) {
            s = s_prime;
        }
        if (compute_residue(v, s) < compute_residue(v, current_best)) {
            current_best = s;
        }
    }

    return compute_residue(v, current_best);
    
}

long long pre_repeated_random(vector<long long> v) {
    uniform_int_distribution<> distribution(0, v.size() - 1);

    vector<long long> current_best(v.size(), 0); 

    // ensure that current best is a valid heap for v
    for (long long i = 0; i < v.size(); i++) {
        long long bucket_index = distribution(rng);
        current_best[bucket_index] += v[i];
    }
    build_heap(current_best);
    long long kk_best = kk(current_best);



    for (long long j = 0; j < 25000; j++) {
        vector<long long> p;
        vector<long long> buckets(v.size(), 0); 

        // for each i, choose which bucket it belongs to randomly
        for (long long i = 0; i < v.size(); i++) {
            long long bucket_index = distribution(rng);
            p.push_back(bucket_index);
            buckets[bucket_index] += v[i];
        }

        build_heap(buckets);
        long long kk_buckets = kk(buckets);
        if (kk_buckets < kk_best) {
            current_best = buckets;
            kk_best = kk_buckets;
        }
    }

    return kk(current_best);
}


long long pre_hill_climbing(vector<long long> v) {
    uniform_int_distribution<> distribution(0, v.size() - 1);

    vector<long long> current_best(v.size(), 0);       // array of sums resulting from best p
    vector<long long> current_best_p;                  // current best p array
    

    // ensure that current best is a valid heap for v
    for (long long i = 0; i < v.size(); i++) {
        long long bucket_index = distribution(rng);
        current_best_p.push_back(bucket_index);
        current_best[bucket_index] += v[i];
    }

    build_heap(current_best);
    long long kk_best = kk(current_best);

    for (long long j = 0; j < 25000; j++) {
        vector<long long> p = pre_neighbor(v, current_best_p);
        vector<long long> buckets(v.size(), 0);  // candidate to beat current_best

        for (long long i = 0; i < p.size(); i++) {    // iterate through all the ELEMENTS a_j
            buckets[p[i]] += v[i];
        }

        build_heap(buckets);
        long long kk_buckets = kk(buckets);

        if (kk_buckets < kk_best) {
            current_best = buckets;
            current_best_p = p;
            kk_best = kk_buckets;
        }
    }

    return kk_best;
}

long long pre_simulated_annealing(vector<long long> v) {
    uniform_int_distribution<> distribution(0, v.size() - 1);
    uniform_real_distribution<> prob_distribution(0, 1);

    vector<long long> current_best(v.size(), 0);       // array of sums resulting from best p
    vector<long long> current_best_p;                  // current best p array

    vector<long long> current(v.size(), 0);
    vector<long long> current_p;

    // ensure that current best is a valid heap for v
    for (long long i = 0; i < v.size(); i++) {
        long long bucket_index = distribution(rng);
        current_best_p.push_back(bucket_index);
        current_best[bucket_index] += v[i];
    }
    current = current_best;                      // analogous to buckets
    current_p = current_best_p;

    build_heap(current_best);
    build_heap(current);

    long long kk_best = kk(current_best);
    long long kk_current = kk_best;

    for (long long j = 0; j < 25000; j++) {
        vector<long long> p = pre_neighbor(v, current_p);
        vector<long long> buckets(v.size(), 0);  // candidate to beat current_best

        for (long long i = 0; i < p.size(); i++) {    // iterate through all the ELEMENTS a_j
            buckets[p[i]] += v[i];
        }

        build_heap(buckets);
        long long kk_buckets = kk(buckets);

        if (kk_buckets < kk_current) {
            current_p = p;
            current = buckets;
            kk_current = kk_buckets;
        } else if (prob_distribution(rng) <= exp(-1 * ((kk_buckets - kk_current) / temperature(j)))) {
            current_p = p;
            current = buckets;
            kk_current = kk_buckets;
        }
        if (kk_current < kk_best) {
            current_best_p = current_p;
            current_best = current;
            kk_best = kk_current;
        }
    }

    return kk_best;
    
}


int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "not enough args" << endl;
        return 1;
    }

    string line;
    int flag = stoi(argv[1]);
    int algorithm = stoll(argv[2]);
    string file_name = argv[3];
    
    fstream reader(file_name);
    long long n_values;

    random_device rd;
    mt19937_64 generate(rd());
    uniform_int_distribution<long long> distribution(1, pow(10, 12));

    if (flag == 1) {
        vector<double> times_0;
        vector<double> times_1;
        vector<double> times_2;
        vector<double> times_3;
        vector<double> times_11;
        vector<double> times_12;
        vector<double> times_13;

        for (int i = 0; i < 50; i++) {
            // fill values randomly
            for (int j = 0; j < 100 ; j++) {
                values.push_back(distribution(generate));
            }

            
            build_heap(values);
            
            auto prev = high_resolution_clock::now();
            cout << kk(values) << "    ";            // kk modifies by value
            duration<double, milli> time_taken = high_resolution_clock::now() - prev;
            times_0.push_back(time_taken.count());

            prev = high_resolution_clock::now();
            cout << repeated_random(values) << "    ";
            time_taken = high_resolution_clock::now() - prev;
            times_1.push_back(time_taken.count());

            prev = high_resolution_clock::now();
            cout << hill_climbing(values) << "    ";
            time_taken = high_resolution_clock::now() - prev;
            times_2.push_back(time_taken.count());

            prev = high_resolution_clock::now();
            cout << simulated_annealing(values) << "    ";
            time_taken = high_resolution_clock::now() - prev;
            times_3.push_back(time_taken.count());

            prev = high_resolution_clock::now();
            cout << pre_repeated_random(values) << "    ";
            time_taken = high_resolution_clock::now() - prev;
            times_11.push_back(time_taken.count());

            prev = high_resolution_clock::now();
            cout << pre_hill_climbing(values) << "    ";
            time_taken = high_resolution_clock::now() - prev;
            times_12.push_back(time_taken.count());

            prev = high_resolution_clock::now();
            cout << pre_simulated_annealing(values) << "    ";
            time_taken = high_resolution_clock::now() - prev;
            times_13.push_back(time_taken.count());
            cout << endl;

            values.clear();
        }
        cout << "DURATIONS:" << endl;
        cout << endl;
        print_vector(times_0);
        print_vector(times_1);
        print_vector(times_2);
        print_vector(times_3);
        print_vector(times_11);
        print_vector(times_12);
        print_vector(times_13);

        return 0;
    }

    if (!reader.is_open()) {
        cout << "invalid file" << endl;
        return 1;
    }

    while (getline(reader, line)) {
        if (!line.empty()) {    
            values.push_back(stoll(line));
        }   
    }

    n_values = values.size();

    build_heap(values);


    if (algorithm == 0) {
        // cout << "KK: " << endl;
        cout << abs(kk(values)) << endl;
    } else if (algorithm == 1) {
        // cout << "Repeated Random: " << endl;
        cout << repeated_random(values) << endl;
    } else if (algorithm == 2) {
        // cout << "Hill Climbing: " << endl;
        cout << hill_climbing(values) << endl;
    } else if (algorithm == 3) {
        // cout << "Simulated Annealing: " << endl;
        cout << simulated_annealing(values) << endl;
    } else if (algorithm == 11) {
        // cout << "Prepartitioned Repeated Random: " << endl;
        cout << pre_repeated_random(values) << endl;
    } else if (algorithm == 12) {
        // cout << "Prepartitioned Hill Climbing: " << endl;
        cout << pre_hill_climbing(values) << endl;
    } else if (algorithm == 13) {
        // cout << "Prepartioned Simulated Annealing: " << endl;
        cout << pre_simulated_annealing(values) << endl;
    }

    
    return 0;
}

