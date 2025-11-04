
#ifndef PROJECT2_IML_HEAP_H
#define PROJECT2_IML_HEAP_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

struct College {
    string name;
    string state;
    double tuition;
    double acceptance_rate;
    double avg_sat;
    string type;
    double score;

    // Comparison for max-heap: bigger score first, then cheaper tuition, then higher SAT
    bool operator<(const College& other) const {
        // first check score
        if (isnan(score) && !isnan(other.score)) return true;
        if (!isnan(score) && isnan(other.score)) return false;
        if (!isnan(score) && !isnan(other.score) && fabs(score - other.score) > 1e-6)
            return score < other.score;

        // then check tuition
        if (isnan(tuition) && !isnan(other.tuition)) return true;
        if (!isnan(tuition) && isnan(other.tuition)) return false;
        if (!isnan(tuition) && !isnan(other.tuition) && fabs(tuition - other.tuition) > 1e-6)
            return tuition > other.tuition; // cheaper is better

        // finally check SAT
        if (isnan(avg_sat) && !isnan(other.avg_sat)) return true;
        if (!isnan(avg_sat) && isnan(other.avg_sat)) return false;
        return avg_sat < other.avg_sat; // higher SAT is better
    }
};

struct CollegeHeap {
    vector<College> heap;

    void push(const College& c);
    College pop();
    bool empty() const { return heap.empty(); }
};

// Function declarations
vector<College> loadCSV(const string& filename);
void computeScores(vector<College>& colleges, double wTuition, double wAcceptance, double wSAT);
vector<College> applyFilters(const vector<College>& all,
                             const string& stateFilter,
                             const string& typeFilter,
                             double maxTuition,
                             double minAcceptance,
                             double minSAT);
void printCollegeShort(const College& c, int rank);
void displayTopColleges(const vector<College>& colleges);


#endif // PROJECT2_IML_HEAP_H
