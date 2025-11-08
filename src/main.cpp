#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include "hashmap_of_hashmaps.h"
#include "Heap.cpp"

// to print a vector of strings
void print_vector(const std::string &title, const std::vector<std::string> &v)
{
	std::cout << title << std::endl;
	if ((int)v.size() == 0)
	{
		std::cout << " (none)" << std::endl;
		return;
	}
	for (int i = 0; i < (int)v.size(); i++)
	{
		std::cout << "  - " << v[i] << std::endl;
	}
}

int main()
{
	hashmap_of_hashmaps test;
	std::string path = "merged_data_100k.csv";
	if (!test.load_file(path))
	{
		std::cout << "Failed to load file" << std::endl;
		return 1;
	}
	// here the universities may be flipped so if they are just use the bottom for the top and vice versa
	print_vector("Top 5 SAT scores:", test.top_n_colleges("SAT scores", 5));
	print_vector("Bottom 5 SAT scores:", test.bottom_n_colleges("SAT scores", 5));

<<<<<<< HEAD
	// College

=======
	for (auto &colle : test.recommend_colleges(5, 1.0f, 1.0f, 1.0f, 1.0f, 1200, "CA", "Public", 30000, 0.3f)){//print the 10 best choices
    	std::cout << colle << std::endl;
	}
>>>>>>> b8762b5ea4ad2a3ecf9b81a194f59a19bdab8eb3
	return 0;
}

//each input for the reccomend_colleges function respectively:
//n: number of colleges user wants (10)
//wTuition: weight for tuition, lower is better (1.0f)
//wAcceptance: weight for acceptance rate, higher is better (1.0f)
//wSAT: weight for SAT score, higher is better (1.0f)
//wUndergrad: weight for undergrad enroll, smaller is better (1.0f)
//min_sat: min sat score (1200)
//state_filter: state user wants ("CA") i tried california because it has more schools
//control_filter: public, private or "" ("Public")
//max_tuition: maximum tuition (30000)
//min_acceptance: minimum acceptance rate (0.3f (30% accept rate))