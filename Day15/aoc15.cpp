#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

using std::cout;
using std::endl;

typedef std::unordered_map<int, int> occ_t;

int main()
{
	std::ifstream infile("input.txt");
	std::string line, num;
	occ_t last_occur, last_occur_init;
	int counter, prev, next;

	// Only 1 line of input, csv
	getline(infile, line);

	/* Read starting numbers & insert into map
	   of number : last occurrence */
	std::stringstream linestream(line);
	counter = 0;
	while(getline(linestream, num, ',')){
		counter++;
		prev = stoi(num);
		last_occur_init[prev] = counter;
	}

	//Part 1, Part 2
	for(int j = 0; j < 2; j++){

		// Assume none of the starting number are duplicate
		int prev_occ = 0;
		last_occur = last_occur_init;

		int target = (j == 0) ? 2020 : 3E7;

		occ_t::iterator it;
		for(int i=counter+1; i <= target; i++){

			if(prev_occ == 0){ // unfound
				next = 0;
			}else{             // found
				next = i - 1 - prev_occ;
			}

			// Look for the last number
			it = last_occur.find(next);
			if(it == last_occur.end()){ // unfound
				prev_occ = 0;
			}else{                      // found
				prev_occ = it -> second;
			}
			last_occur[next] = i;
		}

		cout << "Part " << j+1 << " answer: " << next << endl;
		last_occur.clear();
	}
	return 0;
}
