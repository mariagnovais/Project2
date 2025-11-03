
#ifndef PROJECT2_IML_HEAP_H
#define PROJECT2_IML_HEAP_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;

struct College {
    string name;
    string state;
    double tuition;
    double acceptance_rate;
    double avg_sat;
    string type;
    double score;

    // For priority_queue (max-heap)
    bool operator<(const College& other) const {
        return score < other.score; // higher score = higher priority
    }
};

// Comparator for priority_queue (alternative heap control)
struct CompareCollegeScore {
    bool operator()(const College& a, const College& b) const {
        return a.score < b.score; // max-heap by score
    }
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
void printCollegeDetail(const College& c);
void displayTopColleges(const vector<College>& colleges); // NEW — heap-based ranking

#endif

#endif // PROJECT2_IML_HEAP_H
