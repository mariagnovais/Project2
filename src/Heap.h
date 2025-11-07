
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
    string type;
    string major;
    string cipdesc;
    string creddesc;
    double tuition;
    double acceptance_rate;
    double avg_sat;
    double undergrad_enrollment;
    double cipcode;
    double score;

    // Comparison for max-heap (higher score first)
    bool operator<(const College& other) const {
        if (isnan(score) && !isnan(other.score)) return true;
        if (!isnan(score) && isnan(other.score)) return false;
        return score < other.score;
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
void computeScores(vector<College>& colleges,
                   double wTuition,
                   double wAcceptance,
                   double wSAT,
                   double wUndergrad);

vector<College> applyFilters(const vector<College>& all,
                             const string& stateFilter,
                             const string& typeFilter,
                             const string& majorFilter,
                             double maxTuition,
                             double minAcceptance,
                             double minSAT,
                             int minUndergrad);

void printCollegeShort(const College& c, int rank);
void displayTopColleges(const vector<College>& colleges);


#endif // PROJECT2_IML_HEAP_H
