//
// Created by Caroline Sholar  on 10/29/25.
//

#ifndef PROJECT2_IML_HEAP_H
#define PROJECT2_IML_HEAP_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>
#include <iomanip>
#include <cfloat>
using namespace std;

struct College
{
    string name;
    string state;
    double tuition;
    double acceptanceRate;
    double avgSAT;
    string type;
    double score;

    bool operator<(const College &other) const
    {
        return score < other.score;
    }
};

// Function Declerations
vector<College> loadCSV(const string &filename);
void computerScores(vector<College> &colleges, double wTuition, double wAcceptance, double wSAT);
vector<College> applyFilters(
    const vector<College> &all,
    const string &stateFilter,
    const string &typeFilter,
    double maxTuition,
    double minAcceptance,
    double minSAT);
void printCollegeShort(const College &c, int rank);
void printCollegeDetail(const College &c);

#endif // PROJECT2_IML_HEAP_H
