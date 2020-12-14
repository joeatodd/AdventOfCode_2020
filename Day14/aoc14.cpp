#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <regex>
#include <queue>

using std::cout;
using std::endl;

typedef std::map<long long int, long int> memory_t;
memory_t memory;

char mask[36];
bool verbose = false;

//regex for memory assignment:
std::regex mem_re("mem\\[([0-9]+)\\] = ([0-9]+)");
std::smatch match;

// Set the system's current bitmask
void set_mask(std::string instr){
	//"mask = " is length 7, so offset
	for( int i=0; i<36; i++ ){
		mask[i] = instr[7+i];
	}

	if(verbose) cout << "Mask is : " << mask << endl;
}

std::pair<int, int> read_line(std::string line){
	std::pair<int, int> result;

	std::regex_search( line, match, mem_re );
	result.first = stoi(match[1]);
	result.second = stoi(match[2]);
	return result;
}

// Set a memory address (which may or may not exist) using bitmask
void set_mem_pt1(std::string instr){

	int addr, num;
	// Break instr into memory address & value
	std::tie(addr, num) = read_line(instr);

	// Flip bits from mask as required
	std::bitset<36> bitty(num);
	if(verbose) cout << "Initial bitty: " << bitty << endl;
	for( int i=0; i<36; i++ ){
		switch(mask[i]){
		case 'X': {
			break;
		}
		case '0': {
			bitty[35-i] = 0;
			if(verbose) cout << "Zeroing bit " 
					 << 35-i << endl;
			break;
		}
		case '1': {
			bitty[35-i] = 1;
			if(verbose) cout << "Oneing bit "
					 << 35-i << endl;
			break;
		}
		}
	}
	if(verbose) cout << "Setting addr " << addr 
			 << " to  " << bitty.to_ulong() << endl;
	memory[addr] = bitty.to_ulong();
}

/* For part 2, need to modify the memory address & fill according
to permutations. */
void set_mem_pt2(std::string instr){
	int addr, num;

	std::tie(addr, num) = read_line(instr);

	// Read the address into bitset & mask accordingly
	std::bitset<36> bitty(addr);
	std::string bitstring = bitty.to_string();
	for( int i=0; i<36; i++ ){
		switch(mask[i]){
		case 'X': {
			bitstring.at(i) = 'X';
			break;
		}
		case '0': { // ignore
			break;
		}
		case '1': {
			bitstring.at(i) = '1';
			break;
		}
		}
	}
	if(verbose) cout << "Final bitty: " << bitstring << endl;

	/* For every 'X' in the memory address, need to use both
	   0, 1. Achieve this using a queue. For each X found, 
	   replace with both options and put these on the queue.
	   Once unique (x-less) values are found in queue, push
	   value to the memory address.
	*/
	std::queue<std::string> addresses;
	std::string curr_addr, new_addr;
	bool unique;
	addresses.push(bitstring);
	while(!addresses.empty()){
		unique = true;
		curr_addr = addresses.front();
		addresses.pop();

		for(int i=0; i<curr_addr.size(); i++){
			if(curr_addr.at(i) == 'X'){
				unique = false;
				new_addr = curr_addr;
				new_addr.at(i) = '0';
				addresses.push(new_addr);
				new_addr.at(i) = '1';
				addresses.push(new_addr);
				break;
			}
		}

		if(unique){
			long long int mem_addr = stol(curr_addr, nullptr, 2);
			if(verbose) cout << "Writing num " << num << 
					    " to addr: "<< mem_addr << endl;
			memory[mem_addr] = num;
		}
	}
}

// Sum all values in memory
long int mem_sum(){

	long int sum = 0;
	for(memory_t::iterator it = memory.begin(); 
	    it != memory.end(); it++)
	{
		sum += it -> second;
	}
	return sum;
}

int main()
{
	std::ifstream infile("input.txt");
	std::string line;

	// Mask & write memory addresses
	while(getline(infile, line)){
		if(verbose) cout << "Line: " << line << endl;
		if(line.substr(0,4) == "mask"){
			set_mask(line);
		}else{
			set_mem_pt1(line);
		}
	}


	cout << "Part 1 sum : " << mem_sum() << endl << endl << endl;

	memory.clear();
	infile.clear();
	infile.seekg(0);

	// Part 2 - now we need to modify the memory address
	// rather than the values
	// Mask & write memory addresses
	while(getline(infile, line)){
		if(verbose) cout << "Line: " << line << endl;
		if(line.substr(0,4) == "mask"){
			set_mask(line);
		}else{
			set_mem_pt2(line);
		}
	}

	cout << "Part 2 sum : " << mem_sum() << endl << endl << endl;

	return 0;
}
