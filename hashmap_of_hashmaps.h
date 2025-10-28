#ifndef PROJECT2_HASHMAP_OF_HASHMAPS_H
#define PROJECT2_HASHMAP_OF_HASHMAPS_H

#include <iostream>

int hashFunction(std::string name, int size);{
int key_size = 0;
    for (char c : name) {
key_size +=1;
}
return key_size % size;
};

class hashmap_of_hashmaps {



public:



};


#endif //PROJECT2_HASHMAP_OF_HASHMAPS_H