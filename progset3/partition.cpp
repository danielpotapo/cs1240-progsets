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


int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "not enough args" << endl;
        return 1;
    }

    string line;
    string file_name = argv[3];
    fstream reader(file_name);

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

    return 0;
}

