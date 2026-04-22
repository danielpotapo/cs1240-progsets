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
vector<int> values;


// ./ partition flag algorithm inputfile


void print_vector(vector<int> v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
}

// return index of left element of heap (-1 if not in heap)
int left(vector<int>& heap, int index) {
    if (index * 2 + 1 >= heap.size()) {
        return -1;
    }
    return 2 * index + 1;
}

// return index of right element of heap (-1 if not in heap)
int right(vector<int>& heap, int index) {
    if (index * 2 + 2 >= heap.size()) {
        return -1;
    }
    return 2 * index + 2;
}

void swap(vector<int>& heap, int i, int j) {
    int temp = heap[i];

    heap[i] = heap[j];
    heap[j] = temp;
}

// n is the index of the element we're min heapifying
void max_heapify(vector<int>& heap, int n) {
    int largest;
    int l = left(heap, n);
    int r = right(heap, n);
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

void build_heap(vector<int>& heap) {
    for (int i = heap.size()/2 - 1; i >= 0; i--) {
        max_heapify(heap, i);
    }
}

int extract_max(vector<int>& heap) {
    int max = heap[0];
    heap[0] = heap[heap.size() - 1];
    heap.pop_back();
    max_heapify(heap, 0);
    return max;
}

int peek(vector<int>& heap) {
    return heap[0];
}

int kk(vector<int>& heap) {
    while (heap[1] != 0 || heap[2] != 0) {
        int largest = heap[0];
        int second_largest = -1;
        int sl_index = left(heap, 0);

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
    // print_vector(heap);
    // cout << endl;

    return heap[0];
}




int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "not enough args" << endl;
        return 1;
    }

    string line;
    string file_name = argv[3];
    fstream reader(file_name);
    int n_values;

    if (!reader.is_open()) {
        cout << "invalid file" << endl;
        return 1;
    }

    while (getline(reader, line)) {
        values.push_back(stoi(line));
    }


    for (int i = 0; i < values.size(); i++) {
        cout << values[i] << endl;
    }

    n_values = values.size();

    build_heap(values);

    // for (int i = 0; i < values.size(); i++) {
    //     cout << values[i] << endl;
    // }

    // cout << "MAXIMUMS: " << endl;

    // for (int i = 0; i < n_values; i++) {
    //     cout << extract_max(values) << endl;
    // }


    cout << "KK: " << endl;
    cout << kk(values) << endl;

    return 0;
}

