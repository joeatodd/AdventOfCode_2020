#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <sstream>
#include <assert.h>

using std::endl;
using std::cout;
using std::vector;

typedef std::map<std::string, vector<int>> rules_t;

std::regex rule_re("^([a-z ]+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)");
std::regex mytix_re("^your ticket:");
std::regex othertix_re("^nearby tickets:");
std::smatch match;

// Check that a value is between the two ranges given by the input
bool inrange(int val, vector<int> range){
	return (val >= range[0] && val <= range[1])
		|| (val >= range[2] && val <= range[3]);
}

int main()
{
	std::ifstream infile("input.txt");
	std::string line, val;
	std::stringstream linestream;
	rules_t rulemap;
	vector<int> currRange, myTicket, currTicket;
	vector<vector<int>> otherTickets;
	bool hitMine = false, hitOthers = false, verbose=false;

	while(getline(infile, line)){

		if(verbose) cout << line << endl;

		// Read the rules (2 ranges) for each ticket field into map
		if(std::regex_search( line, match, rule_re )){
			for(int i=2; i<6; i++){
				currRange.push_back(stoi(match[i]));
			}
			rulemap[match[1]] = currRange;
			currRange.clear();
			continue;
		}

		// Segment into ticket sections (mine, others) using bools
		if(std::regex_search( line, match, mytix_re )){
			hitMine = true;
			if(verbose) cout << "Hit Mine" << endl;
			continue;
		}
		if(std::regex_search( line, match, othertix_re )){
			hitOthers = true;
			if(verbose) cout << "Hit Others" << endl;
			continue;
		}

		if(hitOthers){
			std::stringstream linestream(line);

			// linestream << line;
			while(getline(linestream, val, ',')){
				currTicket.push_back(stoi(val));
			}
			otherTickets.push_back(currTicket);
			currTicket.clear();

		}else if(hitMine){

			std::stringstream linestream(line);
                        //TODO - why doesn't this work?
			// linestream << line;
			while(getline(linestream, val, ',')){
				myTicket.push_back(stoi(val));
			}

		}

	}

	// Check each other ticket has a valid field
	int invalid_sum = 0;
	bool valid_ticket, valid_field;
	vector<bool> tix_valid;
	for(auto tix : otherTickets){
		valid_ticket = true;
		for(auto it : tix){

			/* Check if it fits each rule in turn
			   */
			valid_field = false;
			for(auto it2 : rulemap){
				currRange = it2.second;
				if( inrange(it, currRange) ){
					valid_field = true;
					break;
				}
			}
			if(!valid_field){
				invalid_sum += it;
				valid_ticket = false;
			}
		}
		// Construct vector of valid/invalid tickets
		tix_valid.push_back(valid_ticket);
	}

	cout << "Part 1 answer: " << invalid_sum << endl;

	// Delete invalid tickets
	auto it = otherTickets.begin();
	int counter = 0;
	while(it != otherTickets.end()){
		if(!tix_valid[counter]){
			it = otherTickets.erase(it);
		}else{
			it++;
		}
		counter ++;
	}


	int nfields = rulemap.size();

	// Create a boolean vector to mark which ticket entry can be in each spot
	// Rows are rules, cols are positions
	vector<bool> field_poss(nfields*nfields);
	std::fill(field_poss.begin(), field_poss.end(), true);

	// Maps are unordered - make a vector of field names:
	vector<std::string> field_names;
	for (auto it2 : rulemap) field_names.push_back(it2.first);

	// Loop through all tickets, marking off impossible field positions
	for( auto it : otherTickets){ // for each ticket
		for(int i = 0; i < nfields; i++){ // for each field in this ticket
			for(int j = 0; j < nfields; j++){ // for each possible field
				currRange = rulemap[field_names[j]];
				if(!inrange(it[i], currRange)){
					field_poss[j*nfields + i] = false;
				}
			}
		}
	}

	if(verbose){ // Print out the boolean matrix of possible field positions
		cout << "Field names: ";
		for(auto it : field_names){ cout << it << ", ";}
		cout << endl;
		cout << "Field poss: ";
		for(auto it : field_poss){ cout << it << ", ";}
		cout << endl;
	}

	// This leaves us still with several options, so need to work out which field can
	// only go in one place, then knock down others, and repeat
	while(std::count(field_poss.begin(), field_poss.end(), true) > nfields){

		// Iterate fields
		for(int i = 0; i < nfields; i++){

			// Is there only one possible position for this field?
			if(std::count(field_poss.begin()+(i*nfields),
				      field_poss.begin()+((i+1)*nfields),
				      true) == 1){

				// Mark others off
				auto which = std::find(field_poss.begin()+(i*nfields),
					   field_poss.begin()+((i+1)*nfields),
					   true);
				int idx = std::distance(field_poss.begin(), which) % nfields;

				if(verbose) cout << "Single: " << field_names[i] << " " << idx << endl;

				for(int j = 0; j < nfields; j++){
					if(j==i) continue;
					field_poss[j*nfields + idx] = false;
				}
			}
		}

		if(verbose){
			cout << "Field poss: ";
			for(auto it : field_poss){ cout << it << ", ";}
			cout << endl;
		}
	}

	// Check we succeeded
	assert(std::count(field_poss.begin(), field_poss.end(), true) == nfields);

	// Map field names to positions (& print info)
	std::map<std::string, int> field_position;
	for(int i = 0; i < nfields; i++){
		auto which = std::find(field_poss.begin()+(i*nfields),
				       field_poss.begin()+((i+1)*nfields),
				       true);
		int idx = std::distance(field_poss.begin(), which) % nfields;
		field_position[field_names[i]] = idx;

		if(verbose) cout << "Field " << field_names[i] << " position: " << idx << endl;
	}

	long int product = 1;
	for(auto it : field_position){
		std::string name = it.first;
		int pos = it.second;
		if(name.substr(0,9) == "departure"){
			if(verbose) cout << "Ticket pos: " << pos << " my val: " << myTicket[pos] << endl;
			product *= myTicket[pos];
		}
	}

	cout << "Part 2 answer: " << product << endl;

	return 0;
}
