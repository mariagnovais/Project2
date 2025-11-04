#include "Heap.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <queue>
using namespace std;


// Helper: trim spaces
string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

// Split CSV line by comma
vector<string> splitCSV(const string &line) {
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

// Normalize a value between min and max
double normalize(double val, double minv, double maxv) {
    if (isnan(val)) return 0.0;
    if (maxv <= minv) return 0.5;
    return (val - minv) / (maxv - minv);
}

// Compute weighted scores (0–1)
void computeScores(vector<College>& colleges, double wTuition, double wAcceptance, double wSAT) {
    double minT = DBL_MAX, maxT = 0;
    double minA = DBL_MAX, maxA = 0;
    double minS = DBL_MAX, maxS = 0;

    // Find min/max for normalization
    for (auto &c : colleges) {
        if (!isnan(c.tuition)) { minT = min(minT, c.tuition); maxT = max(maxT, c.tuition); }
        if (!isnan(c.acceptance_rate)) { minA = min(minA, c.acceptance_rate); maxA = max(maxA, c.acceptance_rate); }
        if (!isnan(c.avg_sat)) { minS = min(minS, c.avg_sat); maxS = max(maxS, c.avg_sat); }
    }

    for (auto &c : colleges) {
        double tuitionScore = isnan(c.tuition) ? 0 : (maxT - c.tuition) / (maxT - minT);
        double accScore = isnan(c.acceptance_rate) ? 0 : (c.acceptance_rate - minA) / (maxA - minA);
        double satScore = isnan(c.avg_sat) ? 0 : (c.avg_sat - minS) / (maxS - minS);

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

// Print short version (score as 0–100 integer)
void printCollegeShort(const College &c, int rank) {
    if (rank > 0) cout << rank << ". ";
    cout << c.name << " | " << c.state
         << " | $" << fixed << setprecision(2) << c.tuition
         << " | Acc: " << fixed << setprecision(0) << (c.acceptance_rate * 100) << "%"
         << " | SAT: " << fixed << setprecision(0) << c.avg_sat
         << " | " << c.type
         << " | Score: " << fixed << setprecision(0) << (c.score * 100) << "\n";
}

// Print detailed version (score as 0–100 integer)
void printCollegeDetail(const College &c) {
    cout << "Name: " << c.name << "\n"
         << "State: " << c.state << "\n"
         << "Tuition: $" << fixed << setprecision(2) << c.tuition << "\n"
         << "Acceptance Rate: " << fixed << setprecision(0) << (c.acceptance_rate * 100) << "%\n"
         << "Average SAT: " << fixed << setprecision(0) << c.avg_sat << "\n"
         << "Type: " << c.type << "\n"
         << "Score: " << fixed << setprecision(0) << (c.score * 100) << "\n";
}

// Display top 10 using a priority queue (heap)
void displayTopColleges(const vector<College>& colleges) {
    priority_queue<College, vector<College>, CompareCollegeScore> pq;

    for (auto &c : colleges) {
        pq.push(c);
    }

    cout << "\nTop 10 College Matches (by Score):\n";
    int rank = 1;
    while (!pq.empty() && rank <= 10) {
        College top = pq.top();
        pq.pop();
        printCollegeShort(top, rank);
        rank++;
    }
}