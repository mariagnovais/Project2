#ifndef PROJECT2_HASHMAP_OF_HASHMAPS_H
#define PROJECT2_HASHMAP_OF_HASHMAPS_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>


int hashFunction(std::string name, int size);{
int key_size = 0;
    for (char c : name) {
key_size +=1;
}
return key_size % size;
};

class hashmap_of_hashmaps {

std::unordered_map<std::string, std::map<int, std::vector<std::string>>> filter; //the bigger has maps that the keys are categories like, SAT scores, Tuition price, school size.
//unordered map for fast search, map for sorted search and vector for entries that have the same values
public:
hashmap_of_hashmaps(){
    filter["test"];
};

//adder
void add(std::string filt, int value, std::string college_name){
    filter[filt][value].push_back(college_name);
};

//get the certain category
std::map<int, std::vector<std::string>>* get_category(std::string filter){
auto it = this->filter.find(filter);
if (it == this->filter.end()) {
return nullptr;}
return &(it->second);
}

//print the whole category
void print_whole_category(std::string filter){
auto filt = get_category(filter);
if (!filt){
std::cout<<"No filter found"<<std::endl;
return;
}

std::cout<<"Filter: "<< filter <<std::endl;

for (auto&[value, schools] : *filt){
std::cout<< " " << value <<": ";
for (int i = 0; i < (int)schools.size(); i++){
std::cout<<schools[i];
if (i+1 < (int)schools.size()){
std::cout<< "< ";}
}
std::cout<<std::endl;
}
std::cout<<std::endl;
}

//this gets the top n colleges of the hashmap which is already ordered
std::vector<std::string> top_n_colleges(std::string filter, int n){
std::vector<std::string> output;
auto filt = get_category(filter);
if (!filt){
return output;}

for (auto it = filt->rbegin(); it != filt->rend(); ++it){
const std::vector<std::string>& schools = it->second;
for (int i = 0; i < (int)schools.size(); i++){
output.push_back(schools[i]);
if ((int)output.size() == n){
return output;
}
}
}
return output;
}

//and this gets the bottom n colleges
std::vector<std::string> bottom_n_colleges(std::string filter, int n){
std::vector<std::string> output;
auto filt = get_category(filter);
if (!filt){
return output;}

for (auto it = filt->begin(); it != filt->end(); ++it){
const std::vector<std::string>& schools = it->second;
for (int i = 0; i < (int)schools.size(); i++){
output.push_back(schools[i]);
if ((int)output.size() == n){
return output;
}
}
}
return output;
}

//load from csv file

bool load_file(std::string& path, bool has_header = true) {
std::ifstream file(path.c_str());
if (!file.is_open()) {
std::cout<<"Failed to open file "<<path<<std::endl;
return false;}
std::string line;
if (has_header) {
if (!std::getline(file,line)) {
std::cout<<"Failed to read header from file "<<path<<std::endl;}
return false;
}

while (std::getline(file,line)) {
std::vector<std::string> cols;
file_split_line(line,cols);
if ((int)cols.size()<8){ //expected 8 colums
continue;}
std::string college_name = cols[0];
int sat = parse_int_safe(cols[1],-1);
int size = parse_int_safe(cols[6],-1);
int tuition = parse_int_safe(cols[7],-1);

if (sat >= 0){
add("SAT scores",sat,college_name);}
if (size >= 0){
add("school size",size,college_name);}
if (tuition >= 0){
add("tuition",tuition,college_name);}
}
return true;
}

private:
void file_split_line(std::string& s, std::vector<std::string>& out) {
out.clear();
std::string cur;
for (int i = 0; i < (int)s.size(); i++) {
char ch = s[i];
if (ch == ',') {
out.push_back(cur);
cur.clear();
}
else {
cur.push_back(ch);
}
}
out.push_back(cur);
}

static int parse_int_safe(const std::string& s, int fallback) {
int i = 0;
while (i < (int)s.size() && (s[i] == ' ' || s[i] == '\t')) i++;
int j = (int)s.size() - 1;
while (j >= 0 && (s[j] == ' ' || s[j] == '\t')) j--;
if (i > j) {
return fallback;}
std::string t = s.substr(i, j - i + 1);

try {
return std::stoi(t);
}
catch (...) {
return fallback;
}
}

};





#endif //PROJECT2_HASHMAP_OF_HASHMAPS_H