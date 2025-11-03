#ifndef PROJECT2_HASHMAP_OF_HASHMAPS_H
#define PROJECT2_HASHMAP_OF_HASHMAPS_H

#include <iostream>
#include <unordered_map>

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
auto it = filter.find(filter);
if (it == filter.end()) {
return nullptr;}
return &(it->second);
}

//print the whole category
void print_whole_category(std::string filter){
auto filt = get_filter(filter);
if (!filt){
std::cout<<"No filter found"<<std::endl;
return;
}

std::cout<<"Filter: "<< filter <<std::endl;

for (auto&[value, schools] : *filt){
std::cout<< " " << value <<": ";
for (int i = 0; i < schools.size(); i++){
std::cout<<schools[i];
if (i+1 < schools.size()){
std::cout<< "< ";}
}
}
std::cout<<std::endl;
}

std::vector<std::string> top_n_colleges(std::string filter, int n){
std::vector<std::string> output;
auto filt = get_filter(filter);
if (!filt){
return output;}

//this gets the top n colleges of the hashmap which is already ordered
for (auto it = filt->rbegin(); it != filt->rend(); ++it){
std::vector<std::string> schools = it->second;
for (int i = 0; i < (int)schools.size(); i++){
output.push_back(schools[i]);
if ((int)output.size() == n){
return output;
}
}
return output;
}

//and this gets the bottom n colleges
std::vector<std::string> bottom_n_colleges(std::string filter, int n){
std::vector<std::string> output;
auto filt = get_filter(filter);
if (!filt){
return output;}

for (auto it = filt->begin(); it != filt->end(); ++it){
std::vector<std::string> schools = it->second;
for (int i = 0; i < (int)schools.size(); i++){
output.push_back(schools[i]);
if ((int)output.size() == n){
return output;
}
}
return output;
}



#endif //PROJECT2_HASHMAP_OF_HASHMAPS_H