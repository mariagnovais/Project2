#include <iostream>
#include <vector>
#include "hashmap_of_hashmaps.h"

//to print a vector of strings
void print_vector(const std::string& title, const std::vector<std::string>& v) {
    std::cout << title << std::endl;
    if ((int)v.size() == 0) {
        std::cout << " (none)" << std::endl;
        return;
    }
    for (int i = 0; i < (int)v.size(); i++) {
        std::cout << "  - " << v[i] << std::endl;
    }
}


int main() {
	hashmap_of_hashmaps test;
	std::string path = "merged_data_100k.csv";
	if (!test.load_file(path)) {
		std::cout << "Failed to load file" << std::endl;
		return 1;
	}
	//here the universities may be flipped so if they are just use the bottom for the top and vice versa
	print_list("Top 5 SAT scores:", test.top_n_colleges("SAT scores", 5));
	print_list("Bottom 5 SAT scores:", test.bottom_n_colleges("SAT scores", 5));

	return 0;
}