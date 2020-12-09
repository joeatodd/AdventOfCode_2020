
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

int main()
{
	std::ifstream infile("input.txt");
	// std::ifstream infile("eg1.txt");
	std::vector<int> numbers;
	std::string line;
	int pre_len, first, last, weakidx;
	long int target, complement, weakno;
	bool valid;
	pre_len = 25;

	// Read input
	while(std::getline(infile, line)){
		numbers.push_back(stol(line));
	}

	// Seek the weak number
	for(unsigned int idx = pre_len; idx < numbers.size(); idx++){

		// Get the pre-amble for this number (candidates) in a
		// multiset (ordered list, faster lookup)
		first = idx - pre_len;
		last = idx;
		target = numbers[idx];
		std::multiset<int> candidates (numbers.begin() + first,
					       numbers.begin() + last);


		// Iterate all members of preamble less than target value
		// seeking the complement
		valid = false;
		for(std::multiset<int>::iterator it = candidates.begin();
		    it != candidates.lower_bound(target);
		    it++){

			int cand = *it;
			complement = target - cand;

			int count = candidates.count(complement);

			/* If we happen to find a candidate which is *half* the
			   target value, we need to find this number twice, else
			   we just need to find the complement once
			*/
			valid = (complement == cand) ? (count >= 2) : (count >= 1);
			if(valid) break;
		}
		
		if(!valid){
			weakno = target;
			weakidx = idx;
			std::cout << "Part 1 ans: " << weakno << std::endl;
			break;
		}

	}

	/* Part 2 - find a contiguous range of numbers before weakno which
	   is equal to weakno.
	*/

	int i = 0, j = 1;
	int sum = 0;
	sum = numbers[i];
	while(sum != weakno){

		while(sum < weakno){
			sum += numbers[j];
			j++;
		}

		if(sum > weakno){
			sum -= numbers[i];
			i++;
		}

	}

	auto max_it = std::max_element(numbers.begin()+i,
				       numbers.begin()+j);

	auto min_it = std::min_element(numbers.begin()+i,
				       numbers.begin()+j);

	std::cout << "Part 2 ans: " << *min_it + *max_it << std::endl;

	return 0;
}
