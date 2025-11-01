//
// Created by Caroline Sholar  on 10/29/25.
//

#include "Heap.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <cfloat>

#include "Heap.h"
#include <cfloat>

// Splits a CSV line into parts by comma
vector<string> splitCSV(const string &line) {
    vector<string> parts;
    string cur;
    for (char c : line) {
        if (c == ',') {
            parts.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    parts.push_back(cur);
    return parts;
}

// Reads a CSV file and stores colleges
vector<College> loadCSV(const string &filename) {
    vector<College> colleges;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        return colleges;
    }

    string header;
    getline(file, header);

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        auto cols = splitCSV(line);
        if (cols.size() < 6) continue;

        College c;
        c.name = cols[0];
        c.state = cols[1];
        try { c.tuition = stod(cols[2]); } catch (...) { c.tuition = NAN; }
        try {
            c.acceptance_rate = stod(cols[3]);
            if (c.acceptance_rate > 1.5) c.acceptance_rate /= 100.0; // convert %
        } catch (...) { c.acceptance_rate = NAN; }
        try { c.avg_sat = stod(cols[4]); } catch (...) { c.avg_sat = NAN; }
        c.type = cols[5];
        c.score = 0;
        colleges.push_back(c);
    }
    return colleges;
}

// Simple normalization
double normalize(double val, double minv, double maxv) {
    if (isnan(val)) return 0.0;
    if (maxv <= minv) return 0.5;
    return (val - minv) / (maxv - minv);
}

// Compute a weighted score for each college
void computeScores(vector<College>& colleges, double wTuition, double wAcceptance, double wSAT) {
    double minT = DBL_MAX, maxT = 0;
    double minA = DBL_MAX, maxA = 0;
    double minS = DBL_MAX, maxS = 0;

    for (auto &c : colleges) {
        if (!isnan(c.tuition)) { minT = min(minT, c.tuition); maxT = max(maxT, c.tuition); }
        if (!isnan(c.acceptance_rate)) { minA = min(minA, c.acceptance_rate); maxA = max(maxA, c.acceptance_rate); }
        if (!isnan(c.avg_sat)) { minS = min(minS, c.avg_sat); maxS = max(maxS, c.avg_sat); }
    }

    for (auto &c : colleges) {
        double tuitionScore = 1.0 - normalize(c.tuition, minT, maxT);
        double acceptanceScore = normalize(c.acceptance_rate, minA, maxA);
        double satScore = normalize(c.avg_sat, minS, maxS);

        double totalW = wTuition + wAcceptance + wSAT;
        if (totalW <= 0) totalW = 1;

        c.score = (tuitionScore * wTuition + acceptanceScore * wAcceptance + satScore * wSAT) / totalW;
    }
}

// Filter out unwanted colleges
vector<College> applyFilters(const vector<College>& all,
                             const string& stateFilter,
                             const string& typeFilter,
                             double maxTuition,
                             double minAcceptance,
                             double minSAT) {
    vector<College> out;

    auto toLower = [](string s) {
        for (auto &ch : s) ch = tolower(ch);
        return s;
    };

    string fState = toLower(stateFilter);
    string fType = toLower(typeFilter);

    for (auto &c : all) {
        string sState = toLower(c.state);
        string sType = toLower(c.type);

        // state/type flexible matching
        if (!fState.empty() && sState.find(fState) == string::npos) continue;
        if (!fType.empty() && sType.find(fType) == string::npos) continue;

        // Tuition filter (only apply if user provided a positive value)
        if (maxTuition > 0 && !isnan(c.tuition) && c.tuition > maxTuition) continue;

        // Acceptance rate and SAT (default 0 means "no minimum")
        if (!isnan(c.acceptance_rate) && minAcceptance > 0 && c.acceptance_rate < minAcceptance) continue;
        if (!isnan(c.avg_sat) && minSAT > 0 && c.avg_sat < minSAT) continue;

        out.push_back(c);
    }

    return out;
}

void printCollegeShort(const College &c, int rank) {
    if (rank > 0) cout << rank << ". ";
    cout << c.name << " | " << c.state << " | $" << c.tuition
         << " | Acc: " << (c.acceptance_rate * 100) << "%"
         << " | SAT: " << c.avg_sat
         << " | " << c.type
         << " | Score: " << fixed << setprecision(3) << c.score << "\n";
}

void printCollegeDetail(const College &c) {
    cout << "Name: " << c.name << "\n"
         << "State: " << c.state << "\n"
         << "Tuition: $" << c.tuition << "\n"
         << "Acceptance Rate: " << (c.acceptance_rate * 100) << "%\n"
         << "Average SAT: " << c.avg_sat << "\n"
         << "Type: " << c.type << "\n"
         << "Score: " << c.score << "\n";
}


// Main
/*

#include "College.h"

int main() {
    //  Load both CSV files
    vector<College> tuitionData = loadCSV("collegeTuitionComplete copy.csv");
    vector<College> mergedData = loadCSV("MERGED2023_24_PP copy.csv");

    // Combine them into one big vector (you can refine this merge later)
    vector<College> all = tuitionData;
    all.insert(all.end(), mergedData.begin(), mergedData.end());

    if (all.empty()) {
        cerr << "Error: One or both CSV files could not be loaded.\n";
        return 1;
    }

    cout << "Welcome to College Matcher!\n";

    // STEP 1: Ask for filters
    cout << "Enter your preferred state (or press Enter to skip): ";
    string state;
    getline(cin, state);

    cout << "Would you prefer Public or Private? (or press Enter to skip): ";
    string type;
    getline(cin, type);

    cout << "Enter your maximum tuition (or press Enter for no max): ";
    string tuitionStr;
    getline(cin, tuitionStr);
    double maxTuition = tuitionStr.empty() ? -1 : stod(tuitionStr);

    cout << "Enter your minimum acceptance rate (e.g., 0.3 for 30%, or press Enter for no min): ";
    string accStr;
    getline(cin, accStr);
    double minAcceptance = accStr.empty() ? 0 : stod(accStr);

    cout << "Enter your minimum SAT score (or press Enter for no min): ";
    string satStr;
    getline(cin, satStr);
    double minSAT = satStr.empty() ? 0 : stod(satStr);

    // Ask for weights
    cout << "\nSet your weighting preferences (total doesn’t need to equal 1):\n";
    double wTuition, wAcceptance, wSAT;
    cout << "Weight for tuition importance: ";
    cin >> wTuition;
    cout << "Weight for acceptance rate importance: ";
    cin >> wAcceptance;
    cout << "Weight for SAT importance: ";
    cin >> wSAT;

    // Compute scores and apply filters
    computeScores(all, wTuition, wAcceptance, wSAT);
    vector<College> filtered = applyFilters(all, state, type, maxTuition, minAcceptance, minSAT);

    if (filtered.empty()) {
        cout << "No colleges matched your criteria.\n";
        return 0;
    }

    //  Create priority queue and print top 10
    priority_queue<College> pq;
    for (auto &c : filtered) {
        pq.push(c);
    }

    cout << "\nTop 10 College Matches (by Score):\n";
    for (int i = 1; i <= 10 && !pq.empty(); i++) {
        College top = pq.top();
        pq.pop();
        printCollegeShort(top, i);
    }

    return 0;
}
*/
