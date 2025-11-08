#ifndef PROJECT2_HASHMAP_OF_HASHMAPS_H
#define PROJECT2_HASHMAP_OF_HASHMAPS_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>

class hashmap_of_hashmaps
{
    // outer unordered for fast category lookup; inner ordered for sorted numeric keys
    std::unordered_map<std::string, std::map<int, std::vector<std::string>>> filter;
    std::unordered_map<std::string, std::string> name_to_state;
    std::unordered_map<std::string, std::string> name_to_control;

public:
    hashmap_of_hashmaps();

    // adder
    void add(std::string filt, int value, std::string college_name);

    // get the certain category
    std::map<int, std::vector<std::string>> *get_category(std::string filter);

    // print the whole category
    void print_whole_category(std::string filter);

    // top/bottom N by single category
    std::vector<std::string> top_n_colleges(std::string filter, int n);
    std::vector<std::string> bottom_n_colleges(std::string filter, int n);

    // load from csv file
    bool load_file(std::string &path, bool has_header = true);

    // recommend with filters + weights
    std::vector<std::string> recommend_colleges(
        int n,
        float wTuition, float wAcceptance, float wSAT, float wUndergrad,
        int min_sat,
        const std::string& state_filter,
        const std::string& control_filter,
        int max_tuition,
        float min_acceptance
    );

private:
    static std::string to_lower_copy(const std::string& s);

    void file_split_line(std::string &s, std::vector<std::string> &out);

    static int parse_int_safe(const std::string &s, int fallback);
    static bool parse_float_safe(const std::string &s, float& out);

    static std::string header_to_category(const std::string& header);

    void collect_category_values(std::unordered_map<std::string,int>& sat,
                                 std::unordered_map<std::string,int>& adm,
                                 std::unordered_map<std::string,int>& tui,
                                 std::unordered_map<std::string,int>& siz,
                                 int& sat_min, int& sat_max,
                                 int& adm_min, int& adm_max,
                                 int& tui_min, int& tui_max,
                                 int& siz_min, int& siz_max);

    void build_weighted_scores(std::vector<std::pair<float,std::string> >& out,
                               float wTuition, float wAcceptance, float wSAT, float wUndergrad);

    void build_weighted_scores_filtered(std::vector<std::pair<float,std::string> >& out,
                                        float wTuition, float wAcceptance, float wSAT, float wUndergrad,
                                        const std::unordered_set<std::string>& allowed);
};

#endif // PROJECT2_HASHMAP_OF_HASHMAPS_H