//
// Created by Caroline Sholar  on 10/29/25.
//

#include "Heap.h"
#include <fstream>
#include <sstream>
#include <cmath>

// Splits the CSV line into parts by comma
vector<string> splitCSV(const string &line) {
    vector<string> parts;
    string cur;
    for (char c : line) {
        if (c == ",") {
            parts.push_back(cur);
            cur.clear();
        }
        else {
            cur.push_back(c);
        }
    }
}

// Reads a CSV file and stores colleges


/ Filter out unwanted colleges
vector<College> applyFilters(const vector<College>& all,
                             const string& stateFilter,
                             const string& typeFilter,
                             double maxTuition,
                             double minAcceptance,
                             double minSAT) {
    vector<College> out;
    for (auto &c : all) {
        if (!stateFilter.empty() && c.state != stateFilter) continue;
        if (!typeFilter.empty() && c.type != typeFilter) continue;
        if (!isnan(c.tuition) && maxTuition >= 0 && c.tuition > maxTuition) continue;
        if (!isnan(c.acceptance_rate) && minAcceptance >= 0 && c.acceptance_rate < minAcceptance) continue;
        if (!isnan(c.avg_sat) && minSAT >= 0 && c.avg_sat < minSAT) continue;
        out.push_back(c);
    }
    return out;
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