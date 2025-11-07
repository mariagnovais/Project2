#include "Heap.h"
#include <cfloat>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

static string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static vector<string> splitCSV(const string &line) {
    vector<string> parts;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        parts.push_back(trim(item));
    }
    return parts;
}

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
        if (cols.size() < 11) continue;

        College c;
        c.name = cols[0];
        try { c.avg_sat = (cols[1] == "Unknown") ? NAN : stod(cols[1]); } catch (...) { c.avg_sat = NAN; }
        c.state = cols[2];
        c.type = cols[3];
        c.major = cols[4];
        try { c.acceptance_rate = (cols[5] == "Unknown") ? NAN : stod(cols[5]); } catch (...) { c.acceptance_rate = NAN; }
        try { c.undergrad_enrollment = (cols[6] == "Unknown") ? NAN : stod(cols[6]); } catch (...) { c.undergrad_enrollment = NAN; }
        try { c.tuition = (cols[7] == "Unknown") ? NAN : stod(cols[7]); } catch (...) { c.tuition = NAN; }
        try { c.cipcode = (cols[8] == "Unknown") ? NAN : stod(cols[8]); } catch (...) { c.cipcode = NAN; }
        c.cipdesc = cols[9];
        c.creddesc = cols[10];
        c.score = 0;
        colleges.push_back(c);
    }

    return colleges;
}

void computeScores(vector<College>& colleges, double wTuition, double wAcceptance, double wSAT, double wUndergrad) {
    double minT = DBL_MAX, maxT = 0;
    double minA = DBL_MAX, maxA = 0;
    double minS = DBL_MAX, maxS = 0;
    double minU = DBL_MAX, maxU = 0;

    for (auto &c : colleges) {
        if (!isnan(c.tuition)) { minT = min(minT, c.tuition); maxT = max(maxT, c.tuition); }
        if (!isnan(c.acceptance_rate)) { minA = min(minA, c.acceptance_rate); maxA = max(maxA, c.acceptance_rate); }
        if (!isnan(c.avg_sat)) { minS = min(minS, c.avg_sat); maxS = max(maxS, c.avg_sat); }
        if (!isnan(c.undergrad_enrollment)) { minU = min(minU, c.undergrad_enrollment); maxU = max(maxU, c.undergrad_enrollment); }
    }

    double tuitionRange = (maxT - minT == 0) ? 1 : (maxT - minT);
    double acceptanceRange = (maxA - minA == 0) ? 1 : (maxA - minA);
    double satRange = (maxS - minS == 0) ? 1 : (maxS - minS);
    double undergradRange = (maxU - minU == 0) ? 1 : (maxU - minU);

    for (auto &c : colleges) {
        double tuitionScore = isnan(c.tuition) ? 0 : (maxT - c.tuition) / tuitionRange; // lower better
        double accScore = isnan(c.acceptance_rate) ? 0 : (c.acceptance_rate - minA) / acceptanceRange; // higher better
        double satScore = isnan(c.avg_sat) ? 0 : (c.avg_sat - minS) / satRange; // higher better
        double undergradScore = isnan(c.undergrad_enrollment) ? 0 : (maxU - c.undergrad_enrollment) / undergradRange; // smaller better

        double totalW = wTuition + wAcceptance + wSAT + wUndergrad;
        if (totalW <= 0) totalW = 1;

        c.score = (tuitionScore * wTuition +
                   accScore * wAcceptance +
                   satScore * wSAT +
                   undergradScore * wUndergrad) / totalW;
    }
}

vector<College> applyFilters(const vector<College>& all,
                             const string& stateFilter,
                             const string& typeFilter,
                             const string& majorFilter,
                             double maxTuition,
                             double minAcceptance,
                             double minSAT,
                             int minUndergrad) {
    vector<College> out;
    auto toLower = [](string s) { transform(s.begin(), s.end(), s.begin(), ::tolower); return s; };

    string fState = toLower(trim(stateFilter));
    string fType = toLower(trim(typeFilter));
    string fMajor = toLower(trim(majorFilter));

    for (auto &c : all) {
        string sState = toLower(trim(c.state));
        string sType = toLower(trim(c.type));
        string sMajor = toLower(trim(c.major));
        bool skip = false;

        if (!fState.empty() && sState.find(fState) == string::npos) skip = true;
        if (!fType.empty() && sType.find(fType) == string::npos) skip = true;
        if (!fMajor.empty() && sMajor.find(fMajor) == string::npos &&
            toLower(trim(c.cipdesc)).find(fMajor) == string::npos) skip = true;

        if (maxTuition > 0 && !isnan(c.tuition) && c.tuition > maxTuition) skip = true;
        if (!isnan(c.acceptance_rate) && minAcceptance > 0 && c.acceptance_rate < minAcceptance) skip = true;
        if (!isnan(c.avg_sat) && minSAT > 0 && c.avg_sat < minSAT) skip = true;
        if (!isnan(c.undergrad_enrollment) && minUndergrad > 0 && c.undergrad_enrollment < minUndergrad) skip = true;

        if (!skip) out.push_back(c);
    }

    return out;
}

void printCollegeShort(const College &c, int rank) {
    if (rank > 0) cout << rank << ". ";
    cout << c.name << " | " << c.state
         << " | $" << fixed << setprecision(0) << c.tuition
         << " | Acc: " << fixed << setprecision(0) << (c.acceptance_rate * 100) << "%"
         << " | SAT: " << fixed << setprecision(0) << c.avg_sat
         << " | Undergrads: " << fixed << setprecision(0) << c.undergrad_enrollment
         << " | " << c.type
         << " | Major: " << c.cipdesc
         << " | Score: " << fixed << setprecision(1) << (c.score * 100) << "\n";
}

void CollegeHeap::push(const College& c) {
    heap.push_back(c);
    int i = heap.size() - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent] < heap[i]) {
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

void displayTopColleges(const vector<College>& colleges) {
    CollegeHeap heap;
    for (auto &c : colleges)
        if (!isnan(c.score)) heap.push(c);

    int rank = 1;
    int limit = min(10, (int)colleges.size());
    while (!heap.empty() && rank <= limit) {
        College top = heap.pop();
        printCollegeShort(top, rank);
        rank++;
    }
}
/*
 * Main That Works
 * #include "College.h"
#include <iostream>
#include <unordered_set>
#include <string>

using namespace std;

int main() {
    vector<College> all = loadCSV("merged_data_100k.csv");
    if (all.empty()) {
        cerr << "Error: CSV file could not be loaded.\n";
        return 1;
    }

    cout << "Welcome to College Matcher!\n";

    cout << "Enter your preferred state (or press Enter to skip): ";
    string state;
    getline(cin, state);

    cout << "Would you prefer Public or Private? (or press Enter to skip): ";
    string type;
    getline(cin, type);

    cout << "Enter your preferred major (or press Enter to skip): ";
    string major;
    getline(cin, major);

    cout << "Enter your maximum tuition (or press Enter for no max): ";
    string tuitionStr;
    getline(cin, tuitionStr);
    double maxTuition = tuitionStr.empty() ? -1 : stod(tuitionStr);

    cout << "Enter your minimum acceptance rate (0–1, or press Enter for no min): ";
    string accStr;
    getline(cin, accStr);
    double minAcceptance = accStr.empty() ? 0 : stod(accStr);

    cout << "Enter your minimum SAT score (or press Enter for no min): ";
    string satStr;
    getline(cin, satStr);
    double minSAT = satStr.empty() ? 0 : stod(satStr);

    cout << "Enter your minimum undergraduate enrollment (or press Enter to skip): ";
    string minUnderStr;
    getline(cin, minUnderStr);
    int minUndergrad = minUnderStr.empty() ? -1 : stoi(minUnderStr);

    // Weight inputs
    double wTuition, wAcceptance, wSAT, wUndergrad;
    cout << "Weight for tuition importance: ";
    cin >> wTuition;
    cout << "Weight for acceptance rate importance: ";
    cin >> wAcceptance;
    cout << "Weight for SAT importance: ";
    cin >> wSAT;
    cout << "Weight for undergraduate size importance: ";
    cin >> wUndergrad;

    computeScores(all, wTuition, wAcceptance, wSAT, wUndergrad);

    vector<College> filtered = applyFilters(
            all,
            state,
            type,
            major,
            maxTuition,
            minAcceptance,
            minSAT,
            minUndergrad
    );

    cout << "\nColleges matching filters: " << filtered.size() << "\n";
    if (filtered.empty()) return 0;

    // Remove duplicates
    vector<College> uniqueColleges;
    unordered_set<string> seenKeys;
    for (auto &c : filtered) {
        string key = c.name + "|" + c.state;
        if (seenKeys.find(key) == seenKeys.end()) {
            uniqueColleges.push_back(c);
            seenKeys.insert(key);
        }
    }

    displayTopColleges(uniqueColleges);
    return 0;
}
 */