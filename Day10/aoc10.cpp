#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

int total_options = 0;

void recurse_connect(int jolts, std::map<int, std::vector<int>> &connects_to, int target){
	std::vector<int> this_connects;
	this_connects = connects_to[jolts];

	for(auto it = this_connects.begin(); it != this_connects.end(); it++){
		// std::cout << "Jolts " << jolts << " connects to " << *it << std::endl;
		// std::cout << total_options << std::endl;
		if (*it == target){
			total_options++;
			break;}

		recurse_connect(*it, connects_to, target);
	}
}

int main()
{
	std::ifstream infile("input.txt");
	std::string line;
	std::vector<int> numbers;


	// Read input
	while(std::getline(infile, line)){
		numbers.push_back(stol(line));
	}

	std::sort(numbers.begin(), numbers.end());

	/*
	  For Part 1, just need to sort adaptors and count gaps
	  of 1 and 3 jolts, then multiply them.
	 */
	int jolts = 0, one_count = 0, three_count = 0;
	for(auto it = numbers.begin(); it != numbers.end(); it++){

		if(*it - jolts == 1){
			one_count ++;
		}else if (*it - jolts == 3){
			three_count ++;
		}

		jolts = *it;
	}

	// Our device is 3 higher than last joltage adaptor
	int my_joltage = 3 + *(numbers.end()-1);
	three_count ++;
	std::cout << "Part 1 answer: " << one_count * three_count << std::endl;

	/*
	  For part 2, need to count total combinations of 100 adaptors which get
	  from 0 to my_joltage. There are too many combos to do it naively, so
	  I'm looking for independent subsets which are separated by 3 jolts.
	  Then the combinations for each subset can be multiplied.
	*/

	// Create a map describing which adaptors each adaptor can connect to
	std::map<int, std::vector<int>> connects_to;
	std::vector<int> this_connects;
	int jdiff;
	for(auto it = numbers.begin(); it != numbers.end(); it++){

		this_connects.clear();
		for(auto it2 = numbers.begin(); it2 != numbers.end(); it2++){

			jdiff = *it2 - *it;
			if(jdiff > 0 && jdiff <= 3){
				this_connects.push_back(*it2);
			}
		}
		connects_to.insert({*it, this_connects});
	}

	// Get the root connection
	std::vector<int> root_connects;
	for(auto it2 = numbers.begin(); it2 != numbers.end(); it2++){
		jdiff = *it2;
		if(jdiff > 0 && jdiff <= 3){
			root_connects.push_back(*it2);
		}
			
	}
	connects_to.insert({0, root_connects});
	
	// Get the end connection
	for(auto it2 = numbers.begin(); it2 != numbers.end(); it2++){
		jdiff = my_joltage - *it2;
		if(jdiff > 0 && jdiff <= 3){
			connects_to.at(*it2).push_back(my_joltage);
		}
			
	}

	// Need to split into segments which have a 3 jump - then multiply
	// the number of combinations for each set.
	numbers.insert(numbers.begin(),0);
	numbers.push_back(my_joltage);
	long int running_options = 1;
	int start = 0;
	int end = 0;
	for(int i = 1; i < numbers.size(); i++){
		if(numbers[i] - numbers[i-1] == 3){
			end = numbers[i];
			recurse_connect(start, connects_to, end);
			running_options *= total_options;
			total_options = 0;
			start = end;
		}
	}

	std::cout << "Part 2 answer : " << running_options << std::endl;
	return 0;
}
