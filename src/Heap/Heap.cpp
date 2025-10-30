//
// Created by Caroline Sholar  on 10/29/25.
//

#include "Heap.h"
#include <fstream>
#include <sstream>
#include <cmath>

// Calculates how well a college matches the user's preferences
double calculateMatchScore(const College& college, double userSAT, double maxTuition, string preferredState) {
    double score = 0.0;



