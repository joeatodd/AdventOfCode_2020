#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

int main()
{
	ifstream infile("input.txt");
	string str;
	vector<int> nums;
	bool found;

	// Read input
	while(getline(infile, str)){
		nums.push_back(stoi(str));
	}

	// Part 1: sum of two numbers
	found = false;
	for(int num_1 : nums){
		for(int num_2 : nums){
			if(num_1 + num_2 == 2020){
				cout << "Part 1: " << (num_1 * num_2) << endl;
				found = true;
				break;
			}
		}
		if(found) break;
	}

	// Part 2: sum of three numbers
	found = false;
	for(int num_1 : nums){
		for(int num_2 : nums){
			for(int num_3 : nums){
				if(num_1 + num_2 + num_3 == 2020){
					cout << "Part 2: " << (num_1 * num_2 * num_3) << endl;
					found = true;
					break;
				}
			}
			if(found) break;
		}
		if(found) break;
	}
	return 0;
}
