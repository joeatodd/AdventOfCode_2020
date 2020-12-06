#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

typedef set<char> groupq_t;

int main()
{
	ifstream infile("input.txt");
	string line;
	groupq_t group_qs;
	vector<groupq_t> all_groups;

	while(getline(infile, line)){

		// Groups split on blank line
		if(line.empty()){
			all_groups.push_back(group_qs);
			group_qs.clear();
		}else{
			// Construct set of chars
			for(int i=0; i < line.size(); i++){
				group_qs.insert(line[i]);
			}
		}
	}
	all_groups.push_back(group_qs); // Catch the last one (no blank line at eof)

	// The sum of all group sets
	int group_sum = 0;
	for(groupq_t group : all_groups){
		group_sum += group.size();
	}

	cout << "Part 1 answer: " << group_sum << endl;

	// Reset file
	infile.clear();
	infile.seekg(0);

	// Part 2 requires us to compute the intersection of 
	// sets per person in each group.
	vector<vector<groupq_t>> people_groups;
	vector<groupq_t> curr_group;
	groupq_t curr_person;
	while(getline(infile, line)){

		// Blank line, add the latest group of people to the vector of all groups
		if(line.empty()){
			people_groups.push_back(curr_group);
			curr_group.clear();
		}else{
		
			// Construct this person's set
			for(int i=0; i<line.size(); i++){
				curr_person.insert(line[i]);
			}
			curr_group.push_back(curr_person);
			curr_person.clear();
		}
	}
	people_groups.push_back(curr_group);

	int part2_sum = 0;
	groupq_t curr_set, set2;
	vector<char> curr_vec;
	for(vector<groupq_t> curr_group : people_groups){
		curr_set = curr_group.at(0);
		for(int i=1; i < curr_group.size(); i++){
			set2 = curr_group.at(i);

			set_intersection(curr_set.begin(), curr_set.end(),
					 set2.begin(), set2.end(), back_inserter(curr_vec));

			curr_set.clear();

			for(int j=0; j<curr_vec.size(); j++){
				curr_set.insert(curr_vec.at(j));
			}
			curr_vec.clear();

		}
		part2_sum += curr_set.size();

		curr_set.clear();
	}

	cout << "Part 2 ans: " << part2_sum << endl;
	return 0;
}