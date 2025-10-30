//
// Created by Caroline Sholar  on 10/29/25.
//

#ifndef PROJECT2_IML_HEAP_H
#define PROJECT2_IML_HEAP_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct College {
    string name;
    string state;
    double tuition;
    double admissionRate;
    double avgSAT;
    double matchScore;

    // Used for max heap ordering
    bool operator<(const College& other) const {
        return matchScore < other.matchScore;
    }
};

// Function prototypes
double calculateMatchScore(const College& college, double userSAT, double maxTuition, string preferredState);
vector<College> readCollegeData(const string& filename);


#endif //PROJECT2_IML_HEAP_H
