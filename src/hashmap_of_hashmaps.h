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

std::unordered_map<std::string, std::unordered_map> filter; //the bigger has maps that the keys are categories like, SAT scores, Tuition price, school size.

public:



};


#endif //PROJECT2_HASHMAP_OF_HASHMAPS_H