#include "Heap.h"
#include <cfloat>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

// Trim spaces
static string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

// Split CSV line
static vector<string> splitCSV(const string &line) {
    vector<string> parts;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        parts.push_back(trim(item));
    }
    return parts;
}

// Load CSV
vector<College> loadCSV(const string &filename) {
    vector<College> colleges;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        return colleges;
    }

    string header;
    getline(file, header); // skip header

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        auto cols = splitCSV(line);
        if (cols.size() < 8) continue;

        College c;
        c.name = cols[0];
        try { c.avg_sat = (cols[1] == "Unknown") ? NAN : stod(cols[1]); } catch (...) { c.avg_sat = NAN; }
        c.state = cols[2];
        c.type = cols[3];
        try { c.acceptance_rate = (cols[5] == "Unknown") ? NAN : stod(cols[5]); } catch (...) { c.acceptance_rate = NAN; }
        try { c.tuition = (cols[7] == "Unknown") ? NAN : stod(cols[7]); } catch (...) { c.tuition = NAN; }
        c.score = 0;
        colleges.push_back(c);
    }

    return colleges;
}

// Compute weighted scores
void computeScores(vector<College>& colleges, double wTuition, double wAcceptance, double wSAT) {
    double minT = DBL_MAX, maxT = 0;
    double minA = DBL_MAX, maxA = 0;
    double minS = DBL_MAX, maxS = 0;

    for (auto &c : colleges) {
        if (!isnan(c.tuition)) { minT = min(minT, c.tuition); maxT = max(maxT, c.tuition); }
        if (!isnan(c.acceptance_rate)) { minA = min(minA, c.acceptance_rate); maxA = max(maxA, c.acceptance_rate); }
        if (!isnan(c.avg_sat)) { minS = min(minS, c.avg_sat); maxS = max(maxS, c.avg_sat); }
    }

    // Prevent divide-by-zero
    double tuitionRange = (maxT - minT == 0) ? 1 : (maxT - minT);
    double acceptanceRange = (maxA - minA == 0) ? 1 : (maxA - minA);
    double satRange = (maxS - minS == 0) ? 1 : (maxS - minS);

    for (auto &c : colleges) {
        double tuitionScore = isnan(c.tuition) ? 0 : (maxT - c.tuition) / tuitionRange;
        double accScore = isnan(c.acceptance_rate) ? 0 : (c.acceptance_rate - minA) / acceptanceRange;
        double satScore = isnan(c.avg_sat) ? 0 : (c.avg_sat - minS) / satRange;

        double totalW = wTuition + wAcceptance + wSAT;
        if (totalW <= 0) totalW = 1;

        c.score = (tuitionScore * wTuition + accScore * wAcceptance + satScore * wSAT) / totalW;
    }
}

// Apply filters
vector<College> applyFilters(const vector<College>& all,
                             const string& stateFilter,
                             const string& typeFilter,
                             double maxTuition,
                             double minAcceptance,
                             double minSAT) {
    vector<College> out;
    auto toLower = [](string s) { transform(s.begin(), s.end(), s.begin(), ::tolower); return s; };

    string fState = toLower(trim(stateFilter));
    string fType = toLower(trim(typeFilter));

    for (auto &c : all) {
        string sState = toLower(trim(c.state));
        string sType = toLower(trim(c.type));
        bool skip = false;

        if (!fState.empty() && sState.find(fState) == string::npos) skip = true;
        if (!fType.empty() && sType.find(fType) == string::npos) skip = true;
        if (maxTuition > 0 && !isnan(c.tuition) && c.tuition > maxTuition) skip = true;
        if (!isnan(c.acceptance_rate) && minAcceptance > 0 && c.acceptance_rate < minAcceptance) skip = true;
        if (!isnan(c.avg_sat) && minSAT > 0 && c.avg_sat < minSAT) skip = true;

        if (!skip) out.push_back(c);
    }

    return out;
}

// Print college
void printCollegeShort(const College &c, int rank) {
    if (rank > 0) cout << rank << ". ";
    cout << c.name << " | " << c.state
         << " | $" << fixed << setprecision(2) << c.tuition
         << " | Acc: " << fixed << setprecision(0) << (c.acceptance_rate * 100) << "%"
         << " | SAT: " << fixed << setprecision(0) << c.avg_sat
         << " | " << c.type
         << " | Score: " << fixed << setprecision(0) << (c.score * 100) << "\n";
}

// Heap push/pop (max-heap)
void CollegeHeap::push(const College& c) {
    heap.push_back(c);
    int i = heap.size() - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent] < heap[i]) { // child has higher priority
            swap(heap[parent], heap[i]);
            i = parent;
        } else break;
    }
}

College CollegeHeap::pop() {
    if (heap.empty()) throw runtime_error("Heap is empty");
    College top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    int i = 0, n = heap.size();
    while (true) {
        int left = 2 * i + 1, right = 2 * i + 2;
        int largest = i;
        if (left < n && heap[largest] < heap[left]) largest = left;
        if (right < n && heap[largest] < heap[right]) largest = right;
        if (largest == i) break;
        swap(heap[i], heap[largest]);
        i = largest;
    }

    return top;
}

// Display top colleges (top 10)
void displayTopColleges(const vector<College>& colleges) {
    CollegeHeap heap;

    int pushed = 0;
    for (auto &c: colleges) {
        if (!isnan(c.score) && !isnan(c.tuition) && !isnan(c.acceptance_rate) && !isnan(c.avg_sat)) {
            heap.push(c);
            pushed++;
        }
    }
    cout << "Colleges pushed to heap: " << pushed << endl;

    int rank = 1;
    int limit = min(10, (int) colleges.size());
    while (!heap.empty() && rank <= limit) {
        College top = heap.pop();
        printCollegeShort(top, rank);
        rank++;
    }
}