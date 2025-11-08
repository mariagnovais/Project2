#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <sstream>
#include "hashmap_of_hashmaps.h"
#include "Heap.h"
#include <unordered_set>

namespace
{
	bool parse_int(const std::string &value, int &out)
	{
		if (value.empty())
		{
			return false;
		}
		try
		{
			out = std::stoi(value);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	bool parse_float(const std::string &value, float &out)
	{
		if (value.empty())
		{
			return false;
		}
		try
		{
			out = std::stof(value);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	std::string get_arg(int index, int argc, char *argv[])
	{
		if (index >= argc)
		{
			return "";
		}
		return argv[index];
	}
}

int main(int argc, char *argv[])
{
	// Check for 12 arguments
	if (argc < 13)
	{
		std::cerr << "Usage: " << argv[0]
				  << " <csv-path> [state] [control] [max_tuition] [min_acceptance]"
					 " [min_sat] [undergrad_size] [w_tuition] [w_acceptance] [w_sat] [w_undergrad] [algorithm]"
				  << std::endl;
		return 1;
	}

	std::string csv_path = argv[1];

	std::string state_filter = get_arg(2, argc, argv);
	std::string control_filter = get_arg(3, argc, argv);

	int max_tuition = 0;
	parse_int(get_arg(4, argc, argv), max_tuition);

	float min_acceptance = 0.0f;
	parse_float(get_arg(5, argc, argv), min_acceptance);

	int min_sat = 0;
	parse_int(get_arg(6, argc, argv), min_sat);

	int undergrad_filter_val = 0;
	parse_int(get_arg(7, argc, argv), undergrad_filter_val);

	float weight_tuition = 1.0f;
	parse_float(get_arg(8, argc, argv), weight_tuition);

	float weight_acceptance = 1.0f;
	parse_float(get_arg(9, argc, argv), weight_acceptance);

	float weight_sat = 1.0f;
	parse_float(get_arg(10, argc, argv), weight_sat);

	float weight_undergrad = 1.0f;
	parse_float(get_arg(11, argc, argv), weight_undergrad);

	std::string algorithm_choice = get_arg(12, argc, argv);

	const int results_to_show = 5;
	std::ostringstream output;

	if (algorithm_choice == "heap")
	{
		vector<College> all = loadCSV(csv_path);
		if (all.empty())
		{
			std::cerr << "Error: CSV file could not be loaded by Heap.h.\n";
			return 1;
		}

		computeScores(all, weight_tuition, weight_acceptance, weight_sat, weight_undergrad);

		vector<College> filtered = applyFilters(
			all,
			state_filter,
			control_filter,
			"",
			(double)max_tuition,
			(double)min_acceptance,
			(double)min_sat,
			undergrad_filter_val);

		vector<College> uniqueColleges;
		std::unordered_set<std::string> seenKeys;
		for (auto &c : filtered)
		{
			std::string key = c.name + "|" + c.state;
			if (seenKeys.find(key) == seenKeys.end())
			{
				uniqueColleges.push_back(c);
				seenKeys.insert(key);
			}
		}

		if (uniqueColleges.empty())
		{
			output << "No colleges matched your criteria.";
		}
		else
		{
			CollegeHeap heap;
			for (auto &c : uniqueColleges)
			{
				if (!isnan(c.score))
				{
					heap.push(c);
				}
			}

			int count = 0;
			while (!heap.empty() && count < results_to_show)
			{
				College top = heap.pop();
				output << top.name << ", " << top.score;
				if (!heap.empty() && count < results_to_show - 1)
				{
					output << '\n';
				}
				count++;
			}
			if (count == 0)
			{
				output << "No colleges matched your criteria.";
			}
		}
	}
	else
	{
		hashmap_of_hashmaps matcher;
		if (!matcher.load_file(csv_path))
		{
			std::cerr << "Failed to load file: " << csv_path << std::endl;
			return 1;
		}

		std::vector<std::string> recommendations = matcher.recommend_colleges(
			results_to_show,
			weight_tuition,
			weight_acceptance,
			weight_sat,
			weight_undergrad,
			min_sat,
			state_filter,
			control_filter,
			max_tuition,
			min_acceptance);

		if (recommendations.empty())
		{
			output << "No colleges matched your criteria.";
		}
		else
		{
			for (std::size_t i = 0; i < recommendations.size(); ++i)
			{
				output << recommendations[i];
				if (i + 1 < recommendations.size())
				{
					output << '\n';
				}
			}
		}
	}

	std::cout << output.str() << std::endl;
	return 0;
}

// Theron OLD
// to print a vector of strings
// void print_vector(const std::string &title, const std::vector<std::string> &v)
// {
// 	std::cout << title << std::endl;
// 	if ((int)v.size() == 0)
// 	{
// 		std::cout << " (none)" << std::endl;
// 		return;
// 	}
// 	for (int i = 0; i < (int)v.size(); i++)
// 	{
// 		std::cout << "  - " << v[i] << std::endl;
// 	}
// }

// int main()
// {
// 	hashmap_of_hashmaps test;
// 	std::string path = "merged_data_100k.csv";
// 	if (!test.load_file(path))
// 	{
// 		std::cout << "Failed to load file" << std::endl;
// 		return 1;
// 	}
// 	// here the universities may be flipped so if they are just use the bottom for the top and vice versa
// 	print_vector("Top 5 SAT scores:", test.top_n_colleges("SAT scores", 5));
// 	print_vector("Bottom 5 SAT scores:", test.bottom_n_colleges("SAT scores", 5));

// 	for (auto &colle : test.recommend_colleges(5, 1.0f, 1.0f, 1.0f, 1.0f, 1200, "CA", "Public", 30000, 0.3f))
// 	{ // print the 10 best choices
// 		std::cout << colle << std::endl;
// 	}
// 	return 0;
// }

// each input for the reccomend_colleges function respectively:
// n: number of colleges user wants (10)
// wTuition: weight for tuition, lower is better (1.0f)
// wAcceptance: weight for acceptance rate, higher is better (1.0f)
// wSAT: weight for SAT score, higher is better (1.0f)
// wUndergrad: weight for undergrad enroll, smaller is better (1.0f)
// min_sat: min sat score (1200)
// state_filter: state user wants ("CA") i tried california because it has more schools
// control_filter: public, private or "" ("Public")
// max_tuition: maximum tuition (30000)
// min_acceptance: minimum acceptance rate (0.3f (30% accept rate))